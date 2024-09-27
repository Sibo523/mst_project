2mst_project/
│
├── Makefile
├── run_all_features.sh
│
├── src/
│   ├── main.cpp
│   ├── run_all_features.cpp
│   │
│   ├── graph/
│   │   ├── Graph.hpp
│   │   └── Graph.cpp
│   │
│   ├── mst/
│   │   ├── MSTAlgorithm.hpp
│   │   ├── BoruvkaMST.hpp
│   │   ├── BoruvkaMST.cpp
│   │   ├── PrimMST.hpp
│   │   ├── PrimMST.cpp
│   │   ├── KruskalMST.hpp
│   │   ├── KruskalMST.cpp
│   │   ├── TarjanMST.hpp
│   │   └── TarjanMST.cpp
│   │
│   ├── factory/
│   │   ├── MSTFactory.hpp
│   │   └── MSTFactory.cpp
│   │
│   ├── server/
│   │   ├── Server.hpp
│   │   └── Server.cpp
│   │
│   ├── utils/
│   │   ├── Proactor.hpp
│   │   └── Proactor.cpp
│   │
│   └── analysis/
│       ├── MSTAnalysis.hpp
│       └── MSTAnalysis.cpp
│
├── tests/
│   ├── GraphTests.cpp
│   ├── MSTAlgorithmTests.cpp
│   ├── ServerTests.cpp
│   └── AnalysisTests.cpp
│
└── README.md
