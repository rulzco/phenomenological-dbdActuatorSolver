forAll(fluidRegions, i)
{
    fvMesh& mesh = fluidRegions[i];
    volScalarField& phiE = phiEFluid[i];
    label patchID = mesh.boundaryMesh().findPatchID("electrode_exposed");

    // forAll(mesh.boundaryMesh(), patchID)
    // {
    //     string patchName = mesh.boundaryMesh()[patchID].name();

    //     if (patchName.find("electrodes_exposed") == 0)
    //     {
            double Vapp = abs(max(phiE.boundaryField()[patchID])*2/1E3);
            // const scalarField Va = phiE.boundaryField()[patchID];
    //     }
    // }
    // Info<< "Va " << Va << endl;
    // dimensionedScalar maxRhoC("maxRhoC", dimensionSet(0, 0, 0, 0, 0, 0, 0), 0);

    IOdictionary& dbdProperties = dbdPropertiesFluid[i];
    const word dbdParameters(dbdProperties.lookup("dbdParameters"));

    dimensionedScalar& maxRhoC = maxRhoCF[i];
    dimensionedScalar& debyeLen = debyeLenF[i];
    dimensionedScalar baseFreq = baseFreqF[i];

    if ((dbdParameters != "constant") && (dbdParameters != "function"))
    {
        FatalError << "in dbdProperties, unknown dbdParameters type (constant|function)" << nl
            << dbdParameters[i] << abort(FatalError);
    }

    if (dbdParameters == "function")
	{
        // dimensionedScalar mu("mu", dimensionSet(-1, 0, 2, 0, 0, 1, 0), (1.9163*pow(max(mag(E)),-0.25)).value());
        maxRhoC.value() = 4.0E-6*std::pow(Vapp,2.241);
        debyeLen.value() = 0.2*(0.5611*std::atan(-170.3*std::pow(baseFreq.value()/1E3,-5.124))+1.768)*(0.3E-3*Vapp-7.42E-4);
        Info<< maxRhoC << endl;
        Info<< debyeLen << endl;
    }
    else
    {
        Info<< maxRhoC << endl;
        Info<< debyeLen << endl;
    }
}