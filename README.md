# Webserver

## **Team Member Roles and Responsibilities**

### **Person A: Core Server Functionality**

**Responsibilities:**

1. **Network Programming and Event Loop:**
   - **Socket Setup:**
     - Initialize server sockets.
     - Bind sockets to specified IP addresses and ports.
     - Set up the server to listen on multiple ports as per the configuration.
   - **Non-blocking I/O with `poll()` or Equivalent:**
     - Configure sockets for non-blocking operations.
     - Implement the main event loop using `poll()`, `select()`, `epoll()`, or `kqueue()`.
     - Ensure that the server can handle multiple client connections simultaneously without blocking.

2. **Client Connection Management:**
   - **Accepting New Connections:**
     - Accept incoming client connections.
     - Manage client file descriptors.
   - **Data Transmission:**
     - Read from and write to client sockets using non-blocking I/O.
     - Ensure proper handling of partial reads/writes due to non-blocking nature.

3. **Server Resilience and Stability:**
   - **Error Handling:**
     - Handle socket errors gracefully.
     - Ensure the server does not crash under any circumstances, including out-of-memory situations.
   - **Resource Management:**
     - Properly close sockets and free resources to prevent leaks.
     - Implement timeouts and clean-up for inactive connections.

**Why This Division?**

Person A's role focuses on the foundational elements of the server—setting up the networking infrastructure and ensuring efficient and stable client handling. This area requires deep understanding of system calls and non-blocking I/O mechanisms, which is substantial and critical for the server's performance.

---

### **Person B: HTTP Protocol Implementation**

**Responsibilities:**

1. **HTTP Request Parsing:**
   - **Request Line and Headers:**
     - Parse the HTTP method, URI, and HTTP version from the request line.
     - Parse HTTP headers and store them in an accessible format.
     - Handle various header fields, including `Host`, `Content-Length`, `Transfer-Encoding`, etc.
   - **Request Body Handling:**
     - Support reading of the message body for POST and PUT requests.
     - Implement chunked transfer decoding if necessary.

2. **HTTP Response Generation:**
   - **Status Codes and Headers:**
     - Generate appropriate HTTP status codes based on request handling outcomes.
     - Construct HTTP response headers, including `Content-Type`, `Content-Length`, etc.
   - **Response Body:**
     - Serve static files (HTML, CSS, images, etc.) from the server's file system.
     - Implement default error pages when specific error pages are not provided.

3. **HTTP Methods Implementation:**
   - **GET Method:**
     - Retrieve resources identified by the URI.
   - **POST Method:**
     - Handle data submission to the server.
     - Support file uploads and form submissions.
   - **DELETE Method:**
     - Delete the specified resource from the server.

4. **Compliance and Standards:**
   - **HTTP 1.1 Compliance:**
     - Ensure that the server adheres to the HTTP 1.1 specifications as outlined in RFC 2616.
   - **Connection Management:**
     - Handle `Connection` headers (`keep-alive`, `close`).
     - Manage persistent connections appropriately.

**Why This Division?**

Person B is tasked with implementing the core HTTP functionality, which is central to the server's purpose. Parsing requests and generating responses are complex tasks that involve handling numerous edge cases and ensuring compliance with standards, making this role substantial and distinct.

---

### **Person C: Configuration and Advanced Features**

**Responsibilities:**

1. **Configuration File Parsing:**
   - **Syntax Design and Implementation:**
     - Define a clear and flexible syntax for the configuration file, inspired by NGINX.
   - **Parser Development:**
     - Write a parser to read and interpret the configuration file.
     - Validate configuration parameters and provide meaningful error messages.
   - **Configuration Application:**
     - Apply the parsed configuration to modify server behavior dynamically.

2. **Advanced Feature Implementation:**
   - **CGI Execution:**
     - Implement support for running CGI scripts based on file extensions.
     - Set up environment variables and handle the `execve` system call properly.
     - Ensure that the server can interact with external scripts (e.g., PHP, Python).
   - **File Upload Handling:**
     - Allow clients to upload files to the server.
     - Save uploaded files to specified directories as per the configuration.
   - **Directory Listing:**
     - Generate directory indexes when a directory is requested, and directory listing is enabled.
     - Support custom default files (e.g., `index.html`).
   - **HTTP Redirection:**
     - Implement URL redirection based on configuration rules.
     - Handle both temporary and permanent redirects.
   - **Method Restrictions:**
     - Allow configuration of accepted HTTP methods for specific routes.

3. **Testing and Validation:**
   - **Stress Testing:**
     - Write scripts to simulate high-load scenarios.
     - Ensure the server remains stable under stress.
   - **Compatibility Testing:**
     - Test the server with various web browsers.
     - Compare server responses with those from NGINX for consistency.
   - **Edge Case Handling:**
     - Test for unusual or malformed requests and ensure appropriate responses.

**Why This Division?**

Person C's role involves implementing the features that enhance the server beyond basic functionality. Parsing the configuration file and implementing advanced features like CGI and file uploads are complex tasks that require attention to detail. Additionally, taking responsibility for testing ensures that the server meets quality standards.

---

## **Collaboration and Integration**

While each team member has distinct responsibilities, collaboration is crucial:

- **Regular Meetings:**
  - Schedule frequent team meetings to discuss progress, challenges, and integration points.
- **Interface Agreements:**
  - Define clear interfaces between modules. For example:
    - Person A and B need to agree on how parsed requests are passed from the network layer to the HTTP handler.
    - Person B and C need to coordinate on how configuration settings affect HTTP response generation.
- **Code Reviews:**
  - Conduct code reviews to maintain code quality and consistency across the project.
- **Shared Testing:**
  - All team members should participate in testing to validate the entire system, not just their components.

---

## **Why This Division Ensures Equal Workload**

- **Complexity Balance:**
  - Each role has a mix of complex and straightforward tasks.
  - Network programming and non-blocking I/O (Person A) are as challenging as implementing a robust HTTP parser and handler (Person B) and parsing configurations and handling CGIs (Person C).
- **Distinct Skill Sets:**
  - Tasks are divided based on different areas—networking, protocol implementation, and configuration/features—allowing team members to focus on different aspects without overlapping too much.
- **Equal Responsibility:**
  - Each person is responsible for critical components that are essential for the server's functionality.

---

## **Additional Considerations**

- **Documentation:**
  - Each team member should document their code and functionalities.
  - Person C can consolidate documentation for the configuration file and advanced features.
- **Sample Files and Demonstrations:**
  - Provide sample configuration files and web content to showcase server features during evaluations.

---

By dividing the project in this manner, all team members can work independently on different aspects of the server while contributing equally to the overall project. This approach ensures that everyone has ownership of specific components, promotes collaboration during integration, and leads to a well-rounded and functional HTTP server.
