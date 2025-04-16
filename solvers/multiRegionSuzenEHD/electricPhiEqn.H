{    
    const dictionary& pimple = solutionDict.subDict("PIMPLE");
    
    const double tolerancePhi = pimple.subDict("residualControl").subDict("phiE").getOrDefault<double>("tolerance", 1.0e-5);
    const double toleranceRhoc = pimple.subDict("residualControl").subDict("rhoC").getOrDefault<double>("tolerance", 1.0e-4);
    double  phiEfluidInitResidual = 1.0;
    double  phiEsolidInitResidual = 1.0;
    double  rhoCInitResidual = 1.0;
    int     Iter = 0;

    dimensionedScalar clip("clip", dimensionSet(0, -3, 0, 0, 0, 0, 0), 0.0);

    while(rhoCInitResidual > toleranceRhoc)
    {
        forAll(fluidRegions, i)
        {
            volScalarField& rhoC = rhoCFluid[i];
            volScalarField& epsR = epsFluid[i];
            dimensionedScalar debyeLen = debyeLenF[i];

            fvScalarMatrix rhocEqn
            ( 
                fvm::laplacian(epsR,rhoC) == fvm::Sp(1. / (debyeLen * debyeLen),rhoC) 
            ); 
            // rhoC.oldTime();  
            rhoC.correctBoundaryConditions();
            rhoCInitResidual = rhocEqn.solve().initialResidual();
        }
    }

    while((phiEfluidInitResidual > tolerancePhi) && (phiEsolidInitResidual > tolerancePhi) )
    {
        Iter++;

        forAll(fluidRegions, i)
        {
            // #include "setRegionFluidFields.H"
            volScalarField& phiE = phiEFluid[i];
            volScalarField& epsR = epsFluid[i];
            volScalarField& rhoC = rhoCFluid[i];
            volVectorField& E = EFluid[i];
            // dimensionedScalar debyeLen = debyeLenF[i];
            dimensionedScalar maxRhoC = maxRhoCF[i];
            
            fvScalarMatrix phiEqn
            (
                fvm::laplacian(epsR,phiE)  
                ==
                - maxRhoC*rhoC/Foam::constant::electromagnetic::epsilon0
            );

            phiEfluidInitResidual = phiEqn.solve().initialResidual();

            // electric field
            E = -fvc::grad(phiE);
            // electricFlux = fvc::flux(Efluid);
            phiE.correctBoundaryConditions();


        }

        forAll(solidRegions, i)
        {
            volScalarField& phiE = phiESolid[i];
            volScalarField& epsD = epsSolid[i];
            volVectorField& E = ESolid[i];

            fvScalarMatrix phiEqn
            (
                fvm::laplacian(epsD,phiE)
            );

            phiEsolidInitResidual = phiEqn.solve().initialResidual();

            // electric field
            E = -fvc::grad(phiE);

            phiE.correctBoundaryConditions();
            
        }
        
        if (Iter > nOuterCorr) 
        {
            break;
        }

    }

    forAll(fluidRegions, i)
    {   
        const fvMesh& mesh = fluidRegions[i];

        volScalarField& phiE = phiESolid[i];
        volScalarField& rhoC = rhoCFluid[i];
        volVectorField& E = EFluid[i];
        volVectorField& ehdForce = ehdForceFluid[i];
        dimensionedScalar maxRhoC = maxRhoCF[i];
        dimensionedScalar scaleFactor = scaleFactorF[i];

        ehdForce = scaleFactor*maxRhoC*rhoC*E;

        

        // if(DCactive == "on")
        // {
        //     Info<< "Duty Cycle on DC= " << DC << endl;
        //     if(runTime.value() < newT)
        //     {
        //         ehdForce = scaleFactor*maxRhoC*rhoC*E;
        //         // Info<< "old newT" << newT << endl;
        //     }

        //     if(runTime.value() > newT)
        //     {
        //         ehdForce = maxRhoC*ehdForce*0;
        //     }

        //     if(runTime.value() >= newT + Toff )
        //     {
        //         newT = runTime.value() + Ton;
        //         // Info<< "Update newT" << newT << endl;
        //     }
        // }
        // else{ehdForce = scaleFactor*maxRhoC*rhoC*E;}
    //     Info << "Emax = " << max(mag(EFluid[i])).value() << " V/m" << endl;
    //     Info << "Vmax = " << max(phiEFluid[i]).value() << " V" << endl;
    //     Info << "Vmin = " << min(phiEFluid[i]).value() << " V" << endl;

        scalar totalEhdForceX = fvc::domainIntegrate(ehdForce.component(0)).value();
        scalar totalEhdForceY = fvc::domainIntegrate(ehdForce.component(1)).value();
        scalar totalEhdForceMag = Foam::sqrt(pow(totalEhdForceX, 2) + pow(totalEhdForceY, 2));
        // scalar totalPhie = fvc::domainIntegrate(phiE).value();
        // scalar sumEHD = 0.0;
        // scalar sumPhiE = 0.0;

        // forAll(mesh.C(), cellI)
        // {
        //     sumEHD += mag(ehdForce[cellI])*mesh.V()[cellI];
        //     // sumPhiE += phiE[cellI]*mesh.V()[cellI];
        // }

        Info<< "Region (" << mesh.name() << ")"
            << ": int EHD = " << totalEhdForceMag 
            << ": EHD/L = " <<  totalEhdForceMag/5e-3
            // << ", sum sumEHD = " << sumEHD
            << ", max phiE = " << max(phiE).value() << " V"
            << endl;

    }

}