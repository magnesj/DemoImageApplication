/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
// 
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
// 
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
// 
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "MiaApplication.h"
#include "MiuMainWindow.h"


#include <QLocale>

int main(int argc, char *argv[])
{
    MiaApplication app(argc, argv);

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    setlocale(LC_NUMERIC,"C");

    MiuMainWindow window;
    window.setWindowTitle("MinimalApplication");
    window.show();

    return app.exec();

    return 0;
}

