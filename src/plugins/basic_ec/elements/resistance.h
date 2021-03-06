/***************************************************************************
 *    Resistance model                                                     *
 *       used to simulate resistances in KTechLab                          *
 *    Copyright (c) 2010 Zoltan Padrah                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "interfaces/simulator/ielement.h"

namespace KTechLab {

/**
\brief Model for an ideal resistance

This class corresponds 1 to 1 to the Resistor component
*/
class Resistance : public IElement
{
    public:
        /**
        constructor
        \param parentInModel the description of the associated resistor component,
            from the document model
        */
        Resistance(QVariantMap parentInModel);
        /** destructor */
        virtual ~Resistance();

        /**
        set the conductance of the resistance, in Siemens
        \param g the new resistance, expressed by conductance, in Siemens
        */
        void setConductance(const double g);

        /**
        set the resistance of the resistance, in Ohms
        \param r new value of the resistance, in Ohms
        */
        void setResistance(const double r);

        /**
        \return the value of the resistance, in Ohms
        */
        double resistance() const ;
        /**
        \return the value of the conductance of the resistance, in Siemens
        */
        double conductance() const ;

        /**
         action to be taken on a new simulation step
         */
        virtual void actOnSimulationStep(double currentTime);
        /**
         fill the matrix coefficients in the MNA matrix
         */
        virtual void fillMatrixCoefficients();

    private:
        double m_g; ///< conductance the resistance
};

}