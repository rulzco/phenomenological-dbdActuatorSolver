    // Initialise solid field pointer lists
    PtrList<volScalarField> epsSolid(solidRegions.size());
    PtrList<volScalarField> phiESolid(solidRegions.size());
    PtrList<volVectorField> ESolid(solidRegions.size());

    // Populate solid field pointer lists
    forAll(solidRegions, i)
    {
        Info<< "    Adding to epsD\n" << endl;
        epsSolid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "epsD",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i]
            )
        );

        Info<< "    Adding to phiE\n" << endl;
        phiESolid.set
        (
            i,
            new volScalarField
            (
                IOobject
                (
                    "phiE",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::MUST_READ,
                    IOobject::AUTO_WRITE
                ),
                solidRegions[i]
            )
        );

        Info<< "    Adding to E\n" << endl;
        ESolid.set
        (
            i,
            new volVectorField
            (
                IOobject
                (
                    "E",
                    runTime.timeName(),
                    solidRegions[i],
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE
                ),
                -fvc::grad(phiESolid[i])
            )
        );
    }
