NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = .
OBJDIR = obj

SOURCES = main.c utils.c init.c monitor_death.c routine.c checks.c cleanup.c ft_atoi.c eating.c stagger.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

# =============================
# 💧 Memory Leak Check Rule
# =============================
# Usage: make leaks ARGS="philo arguments"
# Example: make leaks ARGS="5 800 200 200"

leaks: $(NAME)
	@echo "🔍 Checking for memory leaks..."
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --quiet \
		--log-file=valgrind_report.txt \
		./$(NAME) $(ARGS) || true
	@echo "📄 Valgrind report saved to valgrind_report.txt"
	@grep -E "definitely lost|indirectly lost|possibly lost" valgrind_report.txt || echo "✅ No leaks found!"

.PHONY: all clean fclean re leaks
