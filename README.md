# dbdActuatorSolver

**Custom OpenFOAM solvers for simulating Dielectric Barrier Discharge (DBD) plasma actuators using a phenomenological electrohydrodynamic (EHD) model.**

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![OpenFOAM](https://img.shields.io/badge/OpenFOAM-v2506%20(ESI%2Fcom)-orange.svg)](https://www.openfoam.com/)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.15330619.svg)](https://doi.org/10.5281/zenodo.15330619)
[![Paper](https://img.shields.io/badge/Paper-J.%20Aerospace%20Eng.-005696.svg)](https://doi.org/10.1061/JAEEEZ.ASENG-4472)

---

## Table of Contents

- [Overview](#overview)
- [Status](#status)
- [Background](#background)
- [Repository Structure](#repository-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [How to Cite](#how-to-cite)
- [Contributing](#contributing)
- [License](#license)
- [Author & Contact](#author--contact)

---

## Overview

`dbdActuatorSolver` provides a pair of custom OpenFOAM applications for simulating the aerodynamic effect of **DBD plasma actuators** without resolving the underlying plasma physics in detail. Instead, the solver uses a **phenomenological electrohydrodynamic (EHD) body-force model**, derived from the Suzen–Huang formulation, to reproduce the induced "ionic wind" that plasma actuators generate near a surface.

By decoupling the electrostatic problem from the flow problem, the solver keeps computational cost low while remaining accurate enough for **flow-control design studies**, making it suitable for:

- Aerodynamic flow-control research
- Plasma aerodynamics
- Electrohydrodynamic propulsion studies
- Control-oriented and parametric actuator studies (e.g., voltage/frequency sweeps)

## Status

⚠️ **This repository is under active development.**

Planned updates include:

- Improved solver stability and performance
- Additional validation/tutorial test cases
- Extended documentation
- Bug fixes and code refactoring

Issues and pull requests describing reproducibility problems are especially welcome while the repository matures.

## Background

The solver follows a **two-domain strategy**:

1. **Domain 1 — Electrostatics:** the electric potential and charge density distribution induced by the actuator are computed, yielding the EHD body-force field (`ehdForce`).
2. **Domain 2 — Flow:** the previously computed `ehdForce` field is injected as a source term into the momentum equation of an incompressible Navier–Stokes solver, reproducing the actuator-induced flow.

The underlying model is based on the **Suzen–Huang** framework, with a calibration approach that accounts for voltage- and frequency-dependent plasma properties and an empirical correction to better reproduce wall-jet behavior. Full derivation, calibration methodology, and validation against experimental data are presented in the peer-reviewed article listed in [How to Cite](#how-to-cite).

## Repository Structure

```
dbdActuatorSolver/
├── solvers/
│   ├── multiRegionSuzenEHD/   # Electrostatic solver -> computes ehdForce
│   └── dbdFlowSimple/         # Flow solver -> uses ehdForce as a momentum source
├── LICENSE
└── README.md
```

> If your local copy includes tutorial/validation cases (e.g., under `tutorials/`), add them to this tree so new users can find a working example immediately — this is one of the items tracked in [Status](#status).

### Solvers

| Solver | Purpose | Output |
|---|---|---|
| `multiRegionSuzenEHD` | Solves the electrostatic problem in its own computational domain | Electric potential, charge density, EHD force field (`ehdForce`) |
| `dbdFlowSimple` | Solves the Navier–Stokes equations with `ehdForce` as a momentum source | Induced velocity/pressure field |

## Prerequisites

- **OpenFOAM.com (ESI release) v2506** or newer
- A sourced OpenFOAM environment (`source /usr/lib/openfoam/openfoamYYMM/etc/bashrc` or equivalent)
- Standard build tools (`wmake`, a C++ compiler compatible with your OpenFOAM build)

> The solvers rely on the standard OpenFOAM finite-volume/multi-region infrastructure, so they should build on closely related ESI versions, but v2506 is the actively tested target.

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/rulzco/phenomenological-dbdActuatorSolver.git
   ```

2. **Move the solvers into your OpenFOAM user applications directory:**

   ```bash
   cp -r phenomenological-dbdActuatorSolver/solvers $WM_PROJECT_USER_DIR/applications
   ```

3. **Source your OpenFOAM environment**, then compile each solver from its own directory:

   ```bash
   cd $WM_PROJECT_USER_DIR/applications/solvers/multiRegionSuzenEHD
   wmake

   cd $WM_PROJECT_USER_DIR/applications/solvers/dbdFlowSimple
   wmake
   ```

4. Confirm both executables are on your `$PATH` (OpenFOAM adds `$WM_PROJECT_USER_DIR/platforms/.../bin` automatically once sourced):

   ```bash
   which multiRegionSuzenEHD dbdFlowSimple
   ```

## Usage

The simulation workflow is run in two sequential steps within a standard OpenFOAM case directory.

### Step 1 — Compute the plasma force

```bash
multiRegionSuzenEHD
```

This solves the electrostatic sub-problem and writes the EHD force field (`ehdForce`) to the case.

### Step 2 — Compute the induced flow

```bash
dbdFlowSimple
```

This reads `ehdForce` and solves the flow equations with it included as a momentum source, producing the actuator-induced velocity and pressure fields.

> Both solvers expect actuator and electrode geometry/parameters (voltage, frequency, dielectric properties) to be set up in the case's `constant`/`0` dictionaries. A worked example case will be added under `tutorials/` — see [Status](#status).

## How to Cite

If you use this solver in your research, please cite the validation article and, if relevant, the software release itself.

**Journal article (model description, implementation, and validation):**

> Bernal-Orozco, R. A., et al. "Phenomenological Modeling of a DBD Plasma Actuator Using an Independent Domain Technique." *Journal of Aerospace Engineering* 39, no. 3 (2026). https://doi.org/10.1061/JAEEEZ.ASENG-4472

```bibtex
@article{BernalOrozco2026JAEEEZ,
  author  = {Bernal-Orozco, Ra{\'u}l Alberto and others},
  title   = {Phenomenological Modeling of a {DBD} Plasma Actuator Using an Independent Domain Technique},
  journal = {Journal of Aerospace Engineering},
  volume  = {39},
  number  = {3},
  year    = {2026},
  doi     = {10.1061/JAEEEZ.ASENG-4472}
}
```

**Conference contribution (control-oriented application — to appear):**

> Bernal-Orozco, R. A., et al. "Modeling and Control-Oriented Simulation of DBD Plasma Actuators for Aerodynamic Performance Enhancement." Presented at the *MMT Symposium 2026* — 60th Anniversary of *Mechanism and Machine Theory*, Porto, Portugal, June 21–23, 2026. *(In press.)*

```bibtex
@inproceedings{BernalOrozco2026MMT,
  author    = {Bernal-Orozco, Ra{\'u}l Alberto and others},
  title     = {Modeling and Control-Oriented Simulation of {DBD} Plasma Actuators for Aerodynamic Performance Enhancement},
  booktitle = {MMT Symposium 2026 -- 60th Anniversary of Mechanism and Machine Theory},
  address   = {Porto, Portugal},
  year      = {2026},
  note      = {To appear}
}
```

**Software release:**

> Bernal-Orozco, R. A. (2025). *rulzco/phenomenological-dbdActuatorSolver: Initial release of multiRegionSuzenEHD solver.* Zenodo. https://doi.org/10.5281/zenodo.15330600

## Contributing

Contributions are welcome — possible areas include:

- Bug fixes
- Additional validation/tutorial cases
- Documentation improvements
- Code optimization

Please open an [issue](../../issues) or submit a [pull request](../../pulls).

## License

This project is distributed under the **GNU General Public License v3.0 (GPL-3.0)**. See [LICENSE](LICENSE) for details.

## Contact

For questions, bug reports, or collaboration inquiries, please open an [issue](../../issues) on this repository.
<!--
## Author & Contact

**Dr. Raúl Alberto Bernal Orozco**
Researcher & Instructor, ESIME Ticomán, Instituto Politécnico Nacional (IPN), Mexico

For questions, bug reports, or collaboration inquiries, please open an [issue](../../issues) on this repository.
 -->
