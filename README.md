# Farm Management System (MFC)

A Windows desktop application for managing a small farm, written in C++ with Microsoft Foundation Classes (MFC). It tracks wheat, corn and rice crops, the harvest and its revenue, and the farm's employees. All data is saved in plain-text files.

## Features

- **Login screen.** The default credentials are `admin` / `admin`.
- **Crops** (wheat, corn, rice):
  - View and edit each crop's variety, field size, quantity, price, and planting and harvesting dates.
  - Advance a crop through its growth stages: Germination → Seedling → Vegetative → Flowering → Filling → Maturity.
  - See the estimated yield (tonnes per acre). Each crop has its own formula: heads per yard and head weight for wheat; ears per acre, kernels per ear, ear weight and shrinkage for corn; panicles per m², grains per panicle and grain weight for rice.
- **Harvest:**
  - Track water and fertilizer amounts and prices.
  - Harvest and store crops, then sell the stored crops to generate revenue.
  - Start a new season, which resets all crops.
- **Employees:** view the employee list and add new employees (name, age, salary).

## Project structure

```
.
├── Farm Management System MFC.sln   # Visual Studio solution
├── CMakeLists.txt                   # CMake build (used for the Linux/Wine build)
├── src/                             # Visual Studio project, C++ sources and resources
│   ├── Crop.h / Crop.cpp            # Crop base class, Wheat/Corn/Rice, Harvest
│   ├── employee.h / employee.cpp    # Employee class
│   ├── *Dlg.h / *Dlg.cpp            # Dialogs: login, home, crops, employees
│   ├── FarmManagementSystemMFC.rc   # Dialog layouts and resources
│   └── res/                         # Icon and extra resources
└── data/                            # Data files the app reads and writes at runtime
```

## Building and running

### Windows (Visual Studio)

1. Install **Visual Studio 2022** with these components:
   - the **Desktop development with C++** workload
   - **C++ MFC for latest v143 build tools (x86 & x64)**, under the optional components
2. Open `Farm Management System MFC.sln`.
3. Select a configuration (for example `Debug | x64`) and press **Ctrl+F5**.

The project sets the debugger's working directory to the repository root, so the app can find the `data/` folder.

### Linux (MSVC under Wine)

MFC can only be compiled with Microsoft's compiler. On Linux, [msvc-wine](https://github.com/mstorsjo/msvc-wine) can run that compiler through Wine. With the toolchain installed in `~/msvc`:

```bash
export PATH=~/msvc/bin/x64:$PATH
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_COMPILER=cl -DCMAKE_RC_COMPILER=rc -DCMAKE_SYSTEM_NAME=Windows
ninja -C build
wine build/FarmManagementSystem.exe
```

Toolchain setup notes:

- Install `msitools`, then install only the packages this project needs:
  ```bash
  ./vsdownload.py --accept-license --architecture x64 --skip-recommended --dest ~/msvc \
      Microsoft.VisualStudio.Component.VC.Tools.x86.x64 \
      Microsoft.VisualStudio.Component.VC.ATLMFC \
      Microsoft.VisualStudio.Component.Windows11SDK.26100
  ./install.sh ~/msvc
  ```
- `install.sh` corrupts the UTF-16 `.rc` files and binary resources in `atlmfc/include`. After installing, replace every non-header file in that folder with a clean copy. To get one, unpack the headers package separately: `./vsdownload.py --accept-license --only-unpack --skip-recommended --dest <tmp> Microsoft.VC.<version>.MFC.Headers.base`.
- In the same folder, add lowercase symlinks for the ribbon style folders (`aqua` → `Aqua`, and the same for `black`, `blue`, `silver` and `windows7`).
- Build in Release mode, because Debug builds need `winbind` under Wine.

## Data files

The app loads its state from `data/` when it starts and saves it back when it closes. **Always start it from the repository root**, or it won't find these files and will crash on startup.

| File | Contents |
|------|----------|
| `Wheat.txt`, `Corn.txt`, `Rice.txt` | One value per line: variety, field size, quantity, price, growth status, dates, then the crop's yield parameters |
| `Harvest.txt` | Fertilizer and water amounts and prices, stored amount of each crop, total revenue |
| `Employee.txt` | One employee per line: `name age salary` |

`Crop` also has functions that append pest, disease, watering and fertilization logs (`* record.txt`) to `data/`. The UI doesn't use them yet, and git ignores these files.
