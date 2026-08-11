# Define the target
TARGET: make

# Set the variables for the makefile
ifeq ($(CXX, "c++") && $(CFLAGS," -std=c++)),

# Set the flags to compile the target file with the specified C++ flags and C flags.
# The flags are passed to the C compiler as arguments, and they are then passed as arguments
# to the make command.
# This allows us to use the C++ standard library and the C standard library
# in a single program.
# The C compiler is called with the following command:

CFLAGS: -Wall, -Wf
# The C compiler flags to include all warnings and stop the compilation if there
# is a warning.
WARN:  # The C++ compiler flag to include all warnings and stop the
# C compilation if there are warnings.

CXXFLAGS:  # The C++ compiler flags.
CFLAGS += $(CXXFLAGS)
CXXFLAGS += CXXFLAGS

make: TARGET TCC++
