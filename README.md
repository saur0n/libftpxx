# libftp++

libftp++ is a C++ wrapper for ftplib which implements file transfer protocol (FTP).

## Building

* Download and build [ftplib](http://nbpfaus.net/~pfau/ftplib/).
* Start `ldconfig` if applicable
* Download `libftp++` (`git clone https://github.com/saur0n/libftpxx.git`)
* Build with `make`
* Install with `make install` (or `sudo make install`)

## Usage examples

Connect to a FTP site:
```
    try {
        // Connect to a FTP site
        ftp::Connection connection("ftp.example.com");
        // Log in with your username and password
        connection.login("o2s_2394924", "tR5o6d82");
        // ... work with the remote filesystem ...
    }
    catch (ftp::ConnectException e) {
        std::cerr << "FTP error: could not connect to server" << std::endl;
    }
    catch (ftp::Exception e) {
        std::cerr << "FTP error: " << e << std::endl;
    }
```
Change and retrieve current directory:
```
    // Change directory to /pub/libftpxx
    connection.cd("/pub/libftpxx");
    // Go up one level
    connection.cdup();
    // Retrieve current directory
    std::cout << "PWD: " << connection.getDirectory() << std::endl;
```
Make and remove directories:
```
    // Make directory «tmp»
    connection.mkdir("tmp");
    // Remove directory «tmp»
    connection.rmdir("tmp");
```
Retrieve remote system type:
```
    // This will print «UNIX» usually
    std::cout << "System type: " << connection.getSystemType() << std::endl;
```
Copy files from/to remote filesystem:
```
    // Copy remote file Makefile from remote system (and give it new name)
    connection.get("Makefile.local", "Makefile", ftp::Connection::BINARY);
    // Copy local file Makefile to remote system (and give it new name)
    connection.put("Makefile", "Makefile.remote", ftp::Connection::BINARY);
```
Read remote file to string:
```
    ftp::DataConnection file(connection, "index.php",
            ftp::DataConnection::FILE_READ, ftp::DataConnection::BINARY);
    std::ostringstream os;
    ssize_t nRead;
    char buffer[128];
    do {
        nRead=file.read(buffer, sizeof(buffer));
        if (nRead>0)
            os.write(buffer, nRead);
    } while (nRead>0);
    std::cout << "Remote file contents:\n" << os.str() << endl;
```
