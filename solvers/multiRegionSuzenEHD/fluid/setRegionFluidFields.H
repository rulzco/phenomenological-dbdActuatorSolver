    
    const fvMesh& mesh = fluidRegions[i];
    dimensionedScalar debyeLen = debyeLenF[i];
    dimensionedScalar baseFreq = baseFreqF[i];
    dimensionedScalar pulseFreq = pulseFreqF[i];
    dimensionedScalar dutyCycle = dutyCycleF[i];
    dimensionedScalar rhoFluid = rhoFluidF[i];

    volScalarField& epsR = epsFluid[i];
    volScalarField& phiE = phiEFluid[i];
    volVectorField& E = EFluid[i];
    volScalarField& rhoC = rhoCFluid[i];
    volVectorField& ehdForce = ehdForceFluid[i];
    
    IOdictionary& physicalProperties = physicalPropertiesFluid[i];
    