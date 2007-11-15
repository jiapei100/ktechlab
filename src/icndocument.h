/***************************************************************************
 *   Copyright (C) 2005 by David Saxton                                    *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ICNDOCUMENT_H
#define ICNDOCUMENT_H

#include "itemdocument.h"

#include <qmap.h>

class Cells;
class CNItem;
class CNItemGroup;
class Connector;
class ECNode;
class FlowContainer;
class Node;
class NodeGroup;

typedef QMap< QString, Node* > NodeMap;
typedef QValueList<QGuardedPtr<Connector> > ConnectorList;
typedef QValueList<QGuardedPtr<Node> > NodeList;
typedef QValueList<NodeGroup*> NodeGroupList;
typedef QValueList<QGuardedPtr<NodeGroup> > GuardedNodeGroupList;

/**
@author David Saxton
*/
class ICNDocument : public ItemDocument
{
Q_OBJECT
public:
	ICNDocument( const QString &caption, const char *name );
	virtual ~ICNDocument();
	
	enum hit_score
	{
		hs_none = 0,
		hs_connector = 4,
		hs_item = 1000
	};

	virtual View *createView( ViewContainer *viewContainer, uint viewAreaId, const char *name = 0l );
	
	/**
	 * Will attempt to create an item with the given id at position p. Some item
	 * (such as PIC/START) have restrictions, and can only have one instance of
	 * themselves on the canvas, and adds the operation to the undo list
	 */
	virtual Item* addItem( const QString &id, const QPoint &p, bool newItem );
	/**
	 * Creates a connector between two nodes, and returns a pointer to it
	 * and adds the operation to the undo list
	 */
	Connector* createConnector( const QString &startNodeId, const QString &endNodeId, QPointList *pointList = 0L );
	/**
	 * short for casting whatever itemWithID(id) returns
	 */
	CNItem* cnItemWithID( const QString &id );
	/**
	 * Returns a pointer to a node on the canvas with the given id,
	 * or NULL if no such node exists
	 */
	Node* nodeWithID( const QString &id );
	/**
	 * Returns a pointer to a Connector on the canvas with the given id,
	 * or NULL if no such Connector exists
	 */
	Connector* connectorWithID( const QString &id );
	/**
	 * Adds a QCanvasItem to the delete list to be deleted,
	 * when flushDeleteList() is called
	 */
	virtual void appendDeleteList( QCanvasItem *qcanvasItem );
	/**
	 * Permantly deletes all items that have been added to the delete list with
	 * the appendDeleteList( QCanvasItem *qcanvasItem ) function.
	 */
	virtual void flushDeleteList();
	/**
	 * Reinherit this function to perform special checks on whether the two
	 * given QCanvasItems (either nodes or connectors or both) can be
	 * connected together.
	 */
	virtual bool canConnect( QCanvasItem *qcanvasItem1, QCanvasItem *qcanvasItem2 ) const;
	virtual void copy();
	virtual void selectAll();


	virtual bool registerItem( QCanvasItem *qcanvasItem );
	/**
	 * Returns a pointer to the 2-dimension array of ICNDocument cells.
	 */
	Cells *cells() const { return m_cells; }
	/**
	 * Adds score to the cells at the given cell referece
	 */
	void addCPenalty( int x, int y, int score );
	/**
	 * If there are two connectors joined to a node, then they can be merged
	 * into one connector. The node will not be removed.
	 * @param node The node between the two connectors
	 * @param noCreate If true, no new connectors will be created
	 * @returns true if it was successful in merging the connectors
	 */
	bool joinConnectors( Node *node );
	static int gridSnap( int pos ); /// Returns 'pos' when snapped to grid
	static QPoint gridSnap( const QPoint &pos );
	/**
	 * Returns true if the CNItem is valid - e.g. will return true for a
	 * component in a circuit, but not in a pic program
	 */
	virtual bool isValidItem( Item *item ) = 0;
	virtual bool isValidItem( const QString &itemId ) = 0;
	ConnectorList getCommonConnectors( const ItemList &list );
	NodeList getCommonNodes( const ItemList &list );
	NodeList nodeList() const;
	const ConnectorList & connectorList() const { return m_connectorList; }
	const GuardedNodeGroupList & nodeGroupList() const { return m_nodeGroupList; }
	virtual ItemGroup *selectList() const;
	/**
	 * Creates a connector from node1 to node2. If pointList is non-null, then the
	 * connector will be assigned those points
	 */
	Connector * createConnector( Node *node1, Node *node2, QPointList *pointList = 0L );
	/**
	 * Splits Connector con into two connectors at point pos2, and creates a connector from the node
	 * to the intersection of the two new connectors. If pointList is non-null, then the new connector
	 * from the node will be assigned those points
	 */
	Connector * createConnector( Node *node, Connector *con, const QPoint &pos2, QPointList *pointList = 0L );
	/**
	 * Splits con1 and con2 into two new connectors each at points pos1 and pos2, and creates a new connector
	 * between the two points of intersection given by pos1 and pos2. If pointList is non-null, then the new
	 * connector between the two points will be assigned those points
	 */
	Connector * createConnector( Connector *con1, Connector *con2, const QPoint &pos1, const QPoint &pos2, QPointList *pointList = 0L );
	/**
	 * Returns the flowcontainer at the given position at the highest level that
	 * is not in the current select list, or 0l if there isn't one
	 */
	FlowContainer *flowContainer( const QPoint &pos );
	/**
	 * Sets the drag (e.g. horizontal arrow) cursor for resizing a CNItem, depending on the corner clicked on
	 */
	void setItemResizeCursor( int cornerType );
	
	void getTranslatable( const ItemList & itemList, ConnectorList * fixedConnectors = 0l, ConnectorList * translatableConnectors = 0l, NodeGroupList * translatableNodeGroups = 0l );
	/**
	 * Reroutes invalidated directors. You shouldn't call this function
	 * directly - instead use ItemDocument::requestEvent.
	 */
	void rerouteInvalidatedConnectors();
	/**
	 * Assigns the orphan nodes into NodeGroups. You shouldn't call this
	 * function directly - instead use ItemDocument::requestEvent.
	 */
	void slotAssignNodeGroups();
	
	virtual void unregisterUID( const QString & uid );
	
public slots:
	/**
	 * Deletes all items in the selected item list, along with associated
	 * connectors, etc, and adds the operation to the undo list
	 */
	virtual void deleteSelection();
	/**
	 * This function looks at all the connectors and the nodes, determines
	 * which ones need rerouting, and then reroutes them
	 */
	void requestRerouteInvalidatedConnectors();
	/**
	 * Remaps the 2-dimension array of ICNDocument cells, and the various
	 * hitscores / etc associated with them. This is used for connector
	 * routing, and should be called after e.g. items have been moved
	 */
	void createCellMap();
	/**
	 * Call this to request NodeGroup reassignment.
	 */
	void slotRequestAssignNG();

signals:
	/**
	 * Emitted when a Connector is added
	 */
	void connectorAdded( Connector *connector );
	/**
	 * Emitted when a Node is added
	 */
	void nodeAdded( Node *node );
	
protected:
	/**
	 * Adds all connector points from the items (used in connector routing).
	 * This only needs to be called when connector(s) need routing.
	 */
	void addAllItemConnectorPoints();
	virtual void fillContextMenu( const QPoint &pos );
	/**
	 * Creates a new NodeGroup to control the node, if there does not already
	 * exist a NodeGroup containing the given node. The associated nodes will
	 * also be added to the NodeGroup.
	 * @returns a pointer to the NodeGroup if one was created, or a pointer to the existing one containing that node
	 */
	NodeGroup* createNodeGroup( Node *node );
	/**
	 * Finds (and deletes if found) the NodeGroup containing the given node.
	 * @returns true if the NodeGroup was found and deleted
	 */
	bool deleteNodeGroup( Node *node );
    
	friend class CanvasEditor;
	
	Cells *m_cells;
	NodeMap m_nodeList;
	ConnectorList m_connectorList;
	CNItemGroup *m_selectList; // Selected objects
	GuardedNodeGroupList m_nodeGroupList;
	
private:
	QCanvasItemList m_itemDeleteList; // List of canvas items to be deleted
};


/**
@author David Saxton
*/
class DirCursor
{
public:
	static DirCursor* self();
	~DirCursor();
	
	static QPixmap leftArrow()
	{
		return self()->m_leftArrow;
	}
	
	static QPixmap rightArrow()
	{
		return self()->m_rightArrow;
	}
	
	static QPixmap upArrow()
	{
		return self()->m_upArrow;
	}
	
	static QPixmap downArrow()
	{
		return self()->m_downArrow;
	}
	
protected:
	DirCursor();
	void initCursors();
	
	static DirCursor *m_self;
	QPixmap m_leftArrow;
	QPixmap m_rightArrow;
	QPixmap m_upArrow;
	QPixmap m_downArrow;
};


#endif