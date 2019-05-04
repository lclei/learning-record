void daemonize(const char *cmd);
int main(int argc, char **argv){
  daemonize("main");
  sleep(110);
}
