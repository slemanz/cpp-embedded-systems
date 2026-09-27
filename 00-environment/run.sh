exec docker run -d -it --name dev_env -v "$PWD"/examples:/work -w /work \
    mahmutbegovic/cpp_in_embedded_systems:v1.1 