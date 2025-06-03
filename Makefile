sources := $(wildcard include/*.h src/*/*.c)

.PHONY: check_format format

format:
	@clang-format --style=file --sort-includes -i $(sources) --verbose

check_format:
	@clang-format --style=file --sort-includes -n $(sources) --verbose -Werror
