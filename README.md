# Farm Management System (MFC)

A Windows desktop application for managing a small farm, written in C++ with Microsoft Foundation Classes (MFC). It tracks wheat, corn and rice crops, the harvest and its revenue, and the farm's employees. All data is saved in plain-text files.

## Features

- **Login screen.** The default credentials are `admin` / `admin`. You can log out from the home page and sign in again.
- **Home page** with an overview of crop growth stages, stored crops, total revenue and the number of employees.
- **Crops** (wheat, corn, rice):
  - Edit each crop's variety, field size, quantity, price, and planting and harvesting dates. Every field is checked before anything is saved, and the message names any field that needs fixing.
  - Move a crop through its growth stages: Not Planted → Germination → Seedling → Vegetative → Flowering → Filling → Maturity.
  - See the estimated yield in tonnes per acre, which updates as you type:
    - wheat: heads per square yard × head weight
    - corn: ears per acre × kernels per ear × kernel weight, less the shrinkage
    - rice: panicles per m² × grains per panicle × grain weight
  - **Field Log** for each crop: water or fertilize it from storage (the amount is taken off the stock), or record a pest infestation or disease outbreak.
- **Storage and sales:**
  - Track water and fertilizer stock and prices.
  - **Harvest Matured Crops** stores every crop that has reached Maturity. It uses the quantity you entered, or the estimated yield for the whole field if the quantity is 0.
  - **Sell All Stored Crops** sells the stored crops at each crop's own price and adds the money to the revenue.
  - **Start New Season** resets all three crops, after you confirm. Stored crops, prices and revenue are kept.
- **Employees:** a table of all employees, with add, update and remove (remove asks you to confirm).

## Project structure

```
.
├── Farm Management System MFC.sln   # Visual Studio solution
├── CMakeLists.txt                   # CMake build (used for the Linux/Wine build)
├── src/                             # Visual Studio project, C++ sources and resources
│   ├── Crop.h / Crop.cpp            # Crop base class, Wheat/Corn/Rice, Harvest
│   ├── employee.h / employee.cpp    # Employee class
│   ├── FarmData.h / FarmData.cpp    # All farm data, loaded at startup and saved on each change
│   ├── UiTheme.h / UiTheme.cpp      # Shared dialog base class, colors and input helpers
│   ├── *Dlg.h / *Dlg.cpp            # Dialogs: login, home, crops, field log, employees
│   ├── FarmManagementSystemMFC.rc   # Dialog layouts and resources
│   └── res/                         # Icon and extra resources
├── defaults/                        # Starting data, copied into data/ on first run
└── data/                            # The app's saved data (created at runtime, ignored by git)
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

The app keeps its state in `data/`. At startup it copies any file that is missing there from `defaults/`, so the first run starts with the sample data. To reset a file, delete it from `data/`. The app saves a file as soon as its data changes (Save, an action button, or an employee change). `data/` is ignored by git.

**Always start the app from the repository root.** If a file cannot be loaded, the app shows which file and line caused the problem, then exits.

| File | Contents |
|------|----------|
| `Wheat.txt`, `Corn.txt`, `Rice.txt` | One value per line: variety, field size, quantity, price, growth status, planting date, harvesting date, then the crop's yield inputs |
| `Harvest.txt` | Fertilizer and water amounts and prices, stored amount of each crop, total revenue |
| `Employee.txt` | One employee per line: `name age salary` (the name may contain spaces) |

These files are in both `defaults/` and `data/`. The Field Log also adds entries to `Crop Watering record.txt`, `Crop Fertilization record.txt`, `Pest Infestation record.txt` and `Crop Disease record.txt` in `data/`.
