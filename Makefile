CC      = cc
CFLAGS  = -Wall -Wextra -I.
BUILD   = build

SRCS    = $(shell find . -name '*.c' -not -path './$(BUILD)/*')
OBJS    = $(SRCS:./%.c=$(BUILD)/%.o)

TARGET  = computerenhance

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS += -g -O0
debug: $(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

test-regress-sim8086-decode: $(TARGET)
	./scripts/test-regress-sim8086.sh decode

test-regress-sim8086-exec: $(TARGET)
	./scripts/test-regress-sim8086.sh exec

test-regress-sim8086: test-regress-sim8086-decode test-regress-sim8086-exec
