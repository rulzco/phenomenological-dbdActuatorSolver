/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2017-2019,2022 OpenCFD Ltd.
-------------------------------------------------------------------------------
    Developed by:  (2025)
    Custom solver for multi-region Suzen-Huang DBD EHD modeling
    https://github.com/rulzco/multiRegionSuzenEHD

License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    multiRegionSuzenEHD

Group
    grpCustomEHDSolvers

Description
    Steady solver for the Suzen-Huang dielectric barrier discharge (DBD) 
    electrohydrodynamic (EHD) model. The solver supports multiple solid and
    fluid regions (multi-region framework), enabling the simulation of
    configurations with multiple electrodes or dielectric arrays.

    The solver computes:
      - Electric potential distribution
      - Electric field
      - Charge density
      - Electrohydrodynamic (EHD) body force

    Designed for EHD flow actuation in both fluid domains, with
    region coupling to handle dielectric and electrode effects.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "regionProperties.H"
#include "fvOptions.H"
#include "loopControl.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote
    (
        "Transient solver for buoyant, turbulent fluid flow and solid heat"
        " conduction with conjugate heat transfer"
        " between solid and fluid regions."
    );

    #define NO_CONTROL
    #define CREATE_MESH createMeshesPostProcess.H
    #include "postProcess.H"

    #include "addCheckCaseOptions.H"
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMeshes.H"
    #include "createFields.H"

    while (runTime.run())
    {

        #include "readPIMPLEControls.H"


        ++runTime;

        Info<< "Time = " << runTime.timeName() << nl << endl;

        #include "dbdParameters.H"
        #include "electricPhiEqn.H"

        runTime.write();

        runTime.printExecutionTime(Info);
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
