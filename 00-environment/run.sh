exec docker run -d -it --name dev_env -v "$PWD"/examples:/work -w /work \
    --user "$(id -u):$(id -g)" \
    -v /etc/passwd:/etc/passwd:ro \
    -v /etc/group:/etc/group:ro \
    mahmutbegovic/cpp_in_embedded_systems:v1.1 