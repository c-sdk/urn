PREFIX ?= /usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
PKGCONFIGDIR ?= $(LIBDIR)/pkgconfig

CC := clang
AR ?= ar
PKG_CONFIG ?= pkg-config

NAME := urn
VERSION := 0.1.0

BUILD_DIR := build
TEST_DIR := t
DEPS_DIR := deps

CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
CPPFLAGS ?= -I. -I$(DEPS_DIR) -I$(DEPS_DIR)/traits -I$(DEPS_DIR)/traits-unit -I$(DEPS_DIR)/trace -I$(DEPS_DIR)/stringify
LDFLAGS ?=
LDLIBS ?=

LIB := $(BUILD_DIR)/lib$(NAME).a
PC_FILE := $(BUILD_DIR)/$(NAME).pc
OBJS := $(BUILD_DIR)/urn.o
DEPS := $(OBJS:.o=.d)
TEST_BIN := $(BUILD_DIR)/test_urn
COMPILE_FLAGS := compile_flags.txt

.PHONY: all clean compile-flags install uninstall test pkg-config

all: $(LIB) $(PC_FILE)

$(BUILD_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(LIB): $(OBJS)
	$(AR) rcs $@ $^

$(PC_FILE): urn.pc.in | $(BUILD_DIR)
	sed \
		-e 's|@PREFIX@|$(PREFIX)|g' \
		-e 's|@LIBDIR@|$(LIBDIR)|g' \
		-e 's|@INCLUDEDIR@|$(INCLUDEDIR)|g' \
		-e 's|@VERSION@|$(VERSION)|g' \
		$< > $@

pkg-config: $(PC_FILE)
	@if command -v $(PKG_CONFIG) >/dev/null 2>&1; then \
		$(PKG_CONFIG) --validate $(PC_FILE); \
	else \
		printf '%s\n' "$(PKG_CONFIG) not found; generated $(PC_FILE) without validation"; \
	fi

$(COMPILE_FLAGS): Makefile
	@printf '%s\n' $(CPPFLAGS) $(CFLAGS) > $@

compile-flags: $(COMPILE_FLAGS)

$(TEST_BIN): $(LIB) $(TEST_DIR)/test_urn.c $(DEPS_DIR)/traits-unit/traits-unit.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(TEST_DIR)/test_urn.c $(DEPS_DIR)/traits-unit/traits-unit.c $(LIB) $(LDLIBS) -o $@

test: $(TEST_BIN)
	$(TEST_BIN)

install: all
	install -d $(DESTDIR)$(INCLUDEDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(PKGCONFIGDIR)
	install -m 0644 urn.h $(DESTDIR)$(INCLUDEDIR)/urn.h
	install -m 0644 $(LIB) $(DESTDIR)$(LIBDIR)/lib$(NAME).a
	install -m 0644 $(PC_FILE) $(DESTDIR)$(PKGCONFIGDIR)/$(NAME).pc

uninstall:
	rm -f $(DESTDIR)$(INCLUDEDIR)/urn.h
	rm -f $(DESTDIR)$(LIBDIR)/lib$(NAME).a
	rm -f $(DESTDIR)$(PKGCONFIGDIR)/$(NAME).pc

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
