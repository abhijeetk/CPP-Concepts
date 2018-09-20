#include <assert.h>
#include <netinet/in.h>
#include <regex>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>
#include <iostream>

using namespace std;

int GetFreePortToListen()
{
  int test_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in test_socket_addr;
  memset(&test_socket_addr, 0, sizeof(test_socket_addr));
  test_socket_addr.sin_port = htons(0);
  test_socket_addr.sin_family = AF_INET;
  bind(test_socket, (struct sockaddr*) &test_socket_addr, sizeof(struct sockaddr_in));
  socklen_t len = sizeof(test_socket_addr);
  getsockname(test_socket, (struct sockaddr*) &test_socket_addr, &len);
  int port = ntohs(test_socket_addr.sin_port);
  cout << port << endl;
  close(test_socket);
  return port;
}

int main() {
    cout << GetFreePortToListen() << endl;
    return 0;
}
