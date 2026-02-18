# HTTP Client for Online Movie Library in C

A **command-line HTTP client** in C that interacts with a REST API for an online movie library. The client reads commands from stdin, builds raw HTTP/1.1 requests, sends them over TCP sockets, and parses JSON responses using the [parson](https://github.com/kgabis/parson) library. Developed as a Computer Networks course assignment.

## Overview

The client supports a two-level authentication system (admin + regular user), manages session cookies and JWT tokens across requests, and provides CRUD operations for the movie library. All HTTP communication is done manually over TCP — no HTTP library is used.

## Features

- Manual HTTP/1.1 request construction (GET and POST)
- Session management via cookies (`Set-Cookie` header extraction)
- JWT Bearer token authorization for library access
- JSON request/response handling via [parson 1.5.3](https://github.com/kgabis/parson)
- Two-level authentication: admin credentials + per-user login

## Supported Commands

| Command | Description |
|---|---|
| `login_admin` | Authenticate as administrator; stores session cookie |
| `add_user` | Add a new user (requires admin session) |
| `get_users` | List all registered users |
| `logout_admin` | Log out the admin and clear the session cookie |
| `login` | Log in as a regular user (requires admin username + credentials) |
| `get_access` | Obtain a JWT token for library access |
| `add_movie` | Add a movie (title, year, description, rating) to the library |
| `exit` | Quit the client |

## Project Structure

```
.
├── client.c / client.h     # stdin parsing and main command loop
├── requests.c / requests.h # HTTP request builders and API functions
├── helper.c / helper.h     # TCP socket, send/receive, cookie/token extraction
├── parson.c / parson.h     # JSON parsing library (external, MIT license)
└── Makefile
```

## Implementation Details

### `helper.c` — Transport Layer

Each API call opens a fresh TCP connection to the server (`conectare()`), sends the request, and closes the socket. The helpers handle partial sends in a loop and dynamically grow the receive buffer for large responses (`receive()`).

- `extrage_cookie()` — parses the `Set-Cookie:` response header and stores the session cookie globally
- `extrage_token()` — locates the HTTP body (past `\r\n\r\n`), parses it as JSON with parson, and extracts the `"token"` field
- `find()` — locates the `\r\n\r\n` separator between HTTP headers and body

### `requests.c` — API Layer

- `build()` — constructs a GET request, appending `Cookie` and `Authorization: Bearer` headers if available
- `build2()` — constructs a POST request with `Content-Type`, `Content-Length`, `Cookie`, and `Authorization` headers plus a JSON body
- All API functions follow the same pattern: build JSON payload with parson → construct HTTP request → open socket → send → receive → check for `HTTP/1.1 4xx` error → process response

### `client.c` — Command Loop

Reads commands line by line from stdin, prompts for required fields, and dispatches to the appropriate function in `requests.c`.

## Building & Running

```bash
make
./client
```

The server address and port are defined in `helper.h`:
```c
#define HOST "63.32.125.183"
#define PORT 8081
```

## Dependencies

- [parson 1.5.3](https://github.com/kgabis/parson) — lightweight JSON parsing library (included in source, MIT license)
# HTTP-Client-Movie-Library-C
