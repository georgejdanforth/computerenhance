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
