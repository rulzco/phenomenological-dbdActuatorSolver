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

}
