NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCDIR = .
OBJDIR = obj

SOURCES = main.c utils.c init.c monitor_death.c routine.c checks.c cleanup.c ft_atoi.c
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

.PHONY: all clean fclean re