/***************************************************************************
 *   Copyright (C) 2003-2004 by David Saxton                               *
 *   david@bluehaze.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ECGROUND_H
#define ECGROUND_H

#include "component.h"

class Circuit;

/**
@short Fixed voltage source
@author David Saxton
*/

class ECGround : public Component {

public:
    ECGround(Circuit &ownerCircuit);
    ~ECGround();

private:
};

#endif
