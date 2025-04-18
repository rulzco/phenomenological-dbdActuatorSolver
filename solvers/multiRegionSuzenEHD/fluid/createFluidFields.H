PtrList<dimensionedScalar> debyeLenF(fluidRegions.size());
PtrList<dimensionedScalar> baseFreqF(fluidRegions.size());
PtrList<dimensionedScalar> pulseFreqF(fluidRegions.size());
PtrList<dimensionedScalar> dutyCycleF(fluidRegions.size());
PtrList<dimensionedScalar> rhoFluidF(fluidRegions.size());
PtrList<dimensionedScalar> maxRhoCF(fluidRegions.size());
PtrList<dimensionedScalar> scaleFactorF(fluidRegions.size());

PtrList<IOdictionary> dbdPropertiesFluid(fluidRegions.size());

// Initialise fluid field pointer lists
PtrList<volScalarField> epsFluid(fluidRegions.size());
PtrList<volScalarField> phiEFluid(fluidRegions.size());
PtrList<volVectorField> EFluid(fluidRegions.size());
PtrList<volScalarField> rhoCFluid(fluidRegions.size());
PtrList<volVectorField> ehdForceFluid(fluidRegions.size());


// Initialise fluid field pointer lists
// PtrList<rhoReactionThermo> thermoFluid(fluidRegions.size());
// PtrList<volScalarField> rhoFluid(fluidRegions.size());
// PtrList<volVectorField> UFluid(fluidRegions.size());
// PtrList<surfaceScalarField> phiFluid(fluidRegions.size());
// PtrList<uniformDimensionedScalarField> hRefFluid(fluidRegions.size());
// PtrList<volScalarField> ghFluid(fluidRegions.size());
// PtrList<surfaceScalarField> ghfFluid(fluidRegions.size());
// PtrList<compressible::turbulenceModel> turbulenceFluid(fluidRegions.size());
// PtrList<CombustionModel<rhoReactionThermo>> reactionFluid(fluidRegions.size());
// PtrList<volScalarField> p_rghFluid(fluidRegions.size());
// PtrList<radiation::radiationModel> radiation(fluidRegions.size());
// PtrList<volScalarField> KFluid(fluidRegions.size());
// PtrList<volScalarField> dpdtFluid(fluidRegions.size());
// PtrList<multivariateSurfaceInterpolationScheme<scalar>::fieldTable>
//     fieldsFluid(fluidRegions.size());
// PtrList<volScalarField> QdotFluid(fluidRegions.size());

// PtrList<fvVectorMatrix> UEqFluid(fluidRegions.size());

// List<scalar> initialMassFluid(fluidRegions.size());
// List<bool> frozenFlowFluid(fluidRegions.size(), false);

// PtrList<IOMRFZoneList> MRFfluid(fluidRegions.size());
// PtrList<fv::options> fluidFvOptions(fluidRegions.size());

// List<label> pRefCellFluid(fluidRegions.size());
// List<scalar> pRefValueFluid(fluidRegions.size());

// PtrList<dimensionedScalar> rhoMinFluid(fluidRegions.size());
// PtrList<dimensionedScalar> rhoMaxFluid(fluidRegions.size());

// PtrList<pressureControl> pressureControls(fluidRegions.size());

// const uniformDimensionedVectorField& g = meshObjects::gravity::New(runTime);

// Populate fluid field pointer lists
forAll(fluidRegions, i)
{

    dbdPropertiesFluid.set
    (
        i,
        new IOdictionary
        (
            IOobject
            (
                "dbdProperties",
                runTime.constant(),
                fluidRegions[i],
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE
            )
        )
    );

    scaleFactorF.set
    (
        i,
        new dimensionedScalar("scaleFactor", dimensionSet(0, 0, 0, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );

    maxRhoCF.set
    (
        i,
        new dimensionedScalar("maxRhoC", dimensionSet(0, 0, 0, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );

    debyeLenF.set
    (
        i,
        new dimensionedScalar("debyeLen", dimensionSet(0, 1, 0, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );

    baseFreqF.set
    (
        i,
        new dimensionedScalar("baseFreq", dimensionSet(0, 0, -1, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );
    
    pulseFreqF.set
    (
        i,
        new dimensionedScalar("pulseFreq", dimensionSet(0, 0, -1, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );
    
    dutyCycleF.set
    (
        i,
        new dimensionedScalar("DC", dimensionSet(0, 0, 0, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );

    rhoFluidF.set
    (
        i,
        new dimensionedScalar("rhoAir", dimensionSet(1, -3, 0, 0, 0, 0, 0), dbdPropertiesFluid[i])
    );
    

    Info<< "    Adding to epsR\n" << endl;
    epsFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "epsR",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    Info<< "    Adding to phiE\n" << endl;
    phiEFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "phiE",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    Info<< "    Adding to E\n" << endl;
    EFluid.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "E",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            -fvc::grad(phiEFluid[i])
        )
    );
    
    Info<< "    Adding to rhoC\n" << endl;
    rhoCFluid.set
    (
        i,
        new volScalarField
        (
            IOobject
            (
                "rhoC",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::MUST_READ,
                IOobject::AUTO_WRITE
            ),
            fluidRegions[i]
        )
    );

    Info<< "    Adding to ehdForce\n" << endl;
    ehdForceFluid.set
    (
        i,
        new volVectorField
        (
            IOobject
            (
                "ehdForce",
                runTime.timeName(),
                fluidRegions[i],
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            rhoCFluid[i]*EFluid[i]
        )
    );


    // Info<< "*** Reading fluid mesh thermophysical properties for region "
    //     << fluidRegions[i].name() << nl << endl;

    // Info<< "    Adding to thermoFluid\n" << endl;
    // thermoFluid.set(i, rhoReactionThermo::New(fluidRegions[i]).ptr());

    // Info<< "    Adding to rhoFluid\n" << endl;
    // rhoFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         IOobject
    //         (
    //             "rho",
    //             runTime.timeName(),
    //             fluidRegions[i],
    //             IOobject::NO_READ,
    //             IOobject::AUTO_WRITE
    //         ),
    //         thermoFluid[i].rho()
    //     )
    // );

    // Info<< "    Adding to UFluid\n" << endl;
    // UFluid.set
    // (
    //     i,
    //     new volVectorField
    //     (
    //         IOobject
    //         (
    //             "U",
    //             runTime.timeName(),
    //             fluidRegions[i],
    //             IOobject::MUST_READ,
    //             IOobject::AUTO_WRITE
    //         ),
    //         fluidRegions[i]
    //     )
    // );

    // Info<< "    Adding to phiFluid\n" << endl;
    // phiFluid.set
    // (
    //     i,
    //     new surfaceScalarField
    //     (
    //         IOobject
    //         (
    //             "phi",
    //             runTime.timeName(),
    //             fluidRegions[i],
    //             IOobject::READ_IF_PRESENT,
    //             IOobject::AUTO_WRITE
    //         ),
    //         linearInterpolate(rhoFluid[i]*UFluid[i])
    //             & fluidRegions[i].Sf()
    //     )
    // );

    // Info<< "    Adding to hRefFluid\n" << endl;
    // hRefFluid.set
    // (
    //     i,
    //     new uniformDimensionedScalarField
    //     (
    //         IOobject
    //         (
    //             "hRef",
    //             runTime.constant(),
    //             fluidRegions[i],
    //             IOobject::READ_IF_PRESENT,
    //             IOobject::NO_WRITE
    //         ),
    //         dimensionedScalar(word::null, dimLength, Zero)
    //     )
    // );

    // dimensionedScalar ghRef
    // (
    //     mag(g.value()) > SMALL
    //   ? g & (cmptMag(g.value())/mag(g.value()))*hRefFluid[i]
    //   : dimensionedScalar("ghRef", g.dimensions()*dimLength, 0)
    // );

    // Info<< "    Adding to ghFluid\n" << endl;
    // ghFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         "gh",
    //         (g & fluidRegions[i].C()) - ghRef
    //     )
    // );

    // Info<< "    Adding to ghfFluid\n" << endl;
    // ghfFluid.set
    // (
    //     i,
    //     new surfaceScalarField
    //     (
    //         "ghf",
    //         (g & fluidRegions[i].Cf()) - ghRef
    //     )
    // );

    // Info<< "    Adding to turbulenceFluid\n" << endl;
    // turbulenceFluid.set
    // (
    //     i,
    //     compressible::turbulenceModel::New
    //     (
    //         rhoFluid[i],
    //         UFluid[i],
    //         phiFluid[i],
    //         thermoFluid[i]
    //     ).ptr()
    // );

    // Info<< "    Adding to reactionFluid\n" << endl;
    // reactionFluid.set
    // (
    //     i,
    //     CombustionModel<rhoReactionThermo>::New
    //     (
    //         thermoFluid[i],
    //         turbulenceFluid[i]
    //     )
    // );

    // p_rghFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         IOobject
    //         (
    //             "p_rgh",
    //             runTime.timeName(),
    //             fluidRegions[i],
    //             IOobject::MUST_READ,
    //             IOobject::AUTO_WRITE
    //         ),
    //         fluidRegions[i]
    //     )
    // );

    // // Force p_rgh to be consistent with p
    // p_rghFluid[i] = thermoFluid[i].p() - rhoFluid[i]*ghFluid[i];

    // fluidRegions[i].setFluxRequired(p_rghFluid[i].name());

    // Info<< "    Adding to radiationFluid\n" << endl;
    // radiation.set
    // (
    //     i,
    //     radiation::radiationModel::New(thermoFluid[i].T())
    // );

    // initialMassFluid[i] = fvc::domainIntegrate(rhoFluid[i]).value();

    // Info<< "    Adding to KFluid\n" << endl;
    // KFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         "K",
    //         0.5*magSqr(UFluid[i])
    //     )
    // );

    // Info<< "    Adding to dpdtFluid\n" << endl;
    // dpdtFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         IOobject
    //         (
    //             "dpdt",
    //             runTime.timeName(),
    //             fluidRegions[i]
    //         ),
    //         fluidRegions[i],
    //         dimensionedScalar(thermoFluid[i].p().dimensions()/dimTime, Zero)
    //     )
    // );

    // Info<< "    Adding to fieldsFluid\n" << endl;
    // fieldsFluid.set
    // (
    //     i,
    //     new multivariateSurfaceInterpolationScheme<scalar>::fieldTable
    // );
    // forAll(thermoFluid[i].composition().Y(), j)
    // {
    //     fieldsFluid[i].add(thermoFluid[i].composition().Y()[j]);
    // }
    // fieldsFluid[i].add(thermoFluid[i].he());

    // Info<< "    Adding to QdotFluid\n" << endl;
    // QdotFluid.set
    // (
    //     i,
    //     new volScalarField
    //     (
    //         IOobject
    //         (
    //             "Qdot",
    //             runTime.timeName(),
    //             fluidRegions[i],
    //             IOobject::READ_IF_PRESENT,
    //             IOobject::AUTO_WRITE
    //         ),
    //         fluidRegions[i],
    //         dimensionedScalar(dimEnergy/dimVolume/dimTime, Zero)
    //     )
    // );

    // const dictionary& pimpleDict =
    //     fluidRegions[i].solutionDict().subDict("PIMPLE");
    // pimpleDict.readIfPresent("frozenFlow", frozenFlowFluid[i]);

    // rhoMaxFluid.set
    // (
    //     i,
    //     new dimensionedScalar("rhoMax", dimDensity, GREAT, pimpleDict)
    // );

    // rhoMinFluid.set
    // (
    //     i,
    //     new dimensionedScalar("rhoMin", dimDensity, Zero, pimpleDict)
    // );

    // pressureControls.set
    // (
    //     i,
    //     new pressureControl(thermoFluid[i].p(), rhoFluid[i], pimpleDict, false)
    // );

    // Info<< "    Adding MRF\n" << endl;
    // MRFfluid.set
    // (
    //     i,
    //     new IOMRFZoneList(fluidRegions[i])
    // );

    // Info<< "    Adding fvOptions\n" << endl;
    // fluidFvOptions.set
    // (
    //     i,
    //     new fv::options(fluidRegions[i])
    // );

    // UEqFluid.set
    // (
    //     i,
    //     new fvVectorMatrix(UFluid[i], dimForce)
    // );

    // turbulenceFluid[i].validate();

    // pRefCellFluid[i] = -1;
    // pRefValueFluid[i] = 0.0;

    // if (p_rghFluid[i].needReference())
    // {
    //     setRefCell
    //     (
    //         thermoFluid[i].p(),
    //         p_rghFluid[i],
    //         pimpleDict,
    //         pRefCellFluid[i],
    //         pRefValueFluid[i]
    //     );
    // }

}
