add_executable(MinecraftCmd 
	"main.cpp"
)

target_link_libraries(MinecraftCmd PRIVATE MinecraftLib)

install(TARGETS MinecraftCmd DESTINATION bin)