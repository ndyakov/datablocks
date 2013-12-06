include_directories(${SRC_DIR_FULL})

set(appsrc
	DataBlocks/main 
	DataBlocks/Cell DataBlocks/CustomStack DataBlocks/Expression
	DataBlocks/FormulaFactory DataBlocks/List DataBlocks/Node
	DataBlocks/Row DataBlocks/Table DataBlocks/functions

)

include_directories(${SRC_DIR_FULL})

add_executable(${EXECUTABLE} ${appsrc})

target_link_libraries(${EXECUTABLE} ${MAIN_LIB})

