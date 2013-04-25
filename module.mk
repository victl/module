##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=module
ConfigurationName      :=Release
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/wangzhy/Workspace/yangbo"
ProjectPath            := "/home/wangzhy/UGV/include/module"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=wangzhy
Date                   :=04/16/2013
CodeLitePath           :="/home/wangzhy/.codelite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(HOME)/UGV/libs/libmodule.a
Preprocessors          :=$(PreprocessorSwitch)_REENTRANT 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="/home/wangzhy/UGV/include/module/module.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
CmpOptions             :=-pthread -O2 -Wall $(Preprocessors)
C_CmpOptions           :=-pthread -O2 -Wall $(Preprocessors)
LinkOptions            := -pthread -O2
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)opencv_imgproc $(LibrarySwitch)opencv_core $(LibrarySwitch)opencv_highgui 
LibPath                := $(LibraryPathSwitch). 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/impl_shm$(ObjectSuffix) $(IntermediateDirectory)/impl_factory$(ObjectSuffix) $(IntermediateDirectory)/log$(ObjectSuffix) $(IntermediateDirectory)/shm$(ObjectSuffix) $(IntermediateDirectory)/module$(ObjectSuffix) $(IntermediateDirectory)/utils$(ObjectSuffix) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects) > $(ObjectsFileList)
	$(ArchiveTool) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList)
	@$(MakeDirCommand) "/home/wangzhy/Workspace/yangbo/.build-release"
	@echo rebuilt > "/home/wangzhy/Workspace/yangbo/.build-release/module"

./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/impl_shm$(ObjectSuffix): impl/shm.cpp $(IntermediateDirectory)/impl_shm$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/impl/shm.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/impl_shm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/impl_shm$(DependSuffix): impl/shm.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/impl_shm$(ObjectSuffix) -MF$(IntermediateDirectory)/impl_shm$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/impl/shm.cpp"

$(IntermediateDirectory)/impl_shm$(PreprocessSuffix): impl/shm.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/impl_shm$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/impl/shm.cpp"

$(IntermediateDirectory)/impl_factory$(ObjectSuffix): impl/factory.cpp $(IntermediateDirectory)/impl_factory$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/impl/factory.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/impl_factory$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/impl_factory$(DependSuffix): impl/factory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/impl_factory$(ObjectSuffix) -MF$(IntermediateDirectory)/impl_factory$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/impl/factory.cpp"

$(IntermediateDirectory)/impl_factory$(PreprocessSuffix): impl/factory.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/impl_factory$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/impl/factory.cpp"

$(IntermediateDirectory)/log$(ObjectSuffix): log.cpp $(IntermediateDirectory)/log$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/log.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/log$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/log$(DependSuffix): log.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/log$(ObjectSuffix) -MF$(IntermediateDirectory)/log$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/log.cpp"

$(IntermediateDirectory)/log$(PreprocessSuffix): log.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/log$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/log.cpp"

$(IntermediateDirectory)/shm$(ObjectSuffix): shm.cpp $(IntermediateDirectory)/shm$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/shm.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/shm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/shm$(DependSuffix): shm.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/shm$(ObjectSuffix) -MF$(IntermediateDirectory)/shm$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/shm.cpp"

$(IntermediateDirectory)/shm$(PreprocessSuffix): shm.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/shm$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/shm.cpp"

$(IntermediateDirectory)/module$(ObjectSuffix): module.cpp $(IntermediateDirectory)/module$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/module.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/module$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/module$(DependSuffix): module.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/module$(ObjectSuffix) -MF$(IntermediateDirectory)/module$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/module.cpp"

$(IntermediateDirectory)/module$(PreprocessSuffix): module.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/module$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/module.cpp"

$(IntermediateDirectory)/utils$(ObjectSuffix): utils.cpp $(IntermediateDirectory)/utils$(DependSuffix)
	$(CompilerName) $(IncludePCH) $(SourceSwitch) "/home/wangzhy/UGV/include/module/utils.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/utils$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/utils$(DependSuffix): utils.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/utils$(ObjectSuffix) -MF$(IntermediateDirectory)/utils$(DependSuffix) -MM "/home/wangzhy/UGV/include/module/utils.cpp"

$(IntermediateDirectory)/utils$(PreprocessSuffix): utils.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/utils$(PreprocessSuffix) "/home/wangzhy/UGV/include/module/utils.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/impl_shm$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/impl_shm$(DependSuffix)
	$(RM) $(IntermediateDirectory)/impl_shm$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/impl_factory$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/impl_factory$(DependSuffix)
	$(RM) $(IntermediateDirectory)/impl_factory$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/log$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/log$(DependSuffix)
	$(RM) $(IntermediateDirectory)/log$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/shm$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/shm$(DependSuffix)
	$(RM) $(IntermediateDirectory)/shm$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/module$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/module$(DependSuffix)
	$(RM) $(IntermediateDirectory)/module$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/utils$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/utils$(DependSuffix)
	$(RM) $(IntermediateDirectory)/utils$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "/home/wangzhy/Workspace/yangbo/.build-release/module"


