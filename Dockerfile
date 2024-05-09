# Use Fedora as the base image
FROM fedora:latest

# Install SFML and other dependencies
RUN dnf install -y gcc-c++ SFML-devel

# Set the working directory inside the container
WORKDIR /app

# Copy the entire project directory into the container
COPY . .

# Compile your SFML project
RUN g++ main.cpp -o me -lsfml-graphics -lsfml-window -lsfml-system

# Set permissions for the executable
RUN chmod +x me
RUN chmod -R 755 /app

# Install X11-related dependencies
RUN dnf install -y xorg-x11-server-Xorg xorg-x11-xauth

# Set the DISPLAY environment variable
ENV DISPLAY=:0

# Specify the command to run your SFML project
CMD ["./me"]
