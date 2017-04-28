/*******************************************************************************
 *  FTP access library (ftplib C++ wrapper)
 *  
 *  © 2017, Sauron
 ******************************************************************************/

#include <iostream>
#include "ftp++.hpp"

#define BUFSIZE 1024
#define FTPLIB(call) \
    if (0==call) \
        throw FTPException(getLastResponse());

FTPConnectionBase::~FTPConnectionBase() {
    FtpClose(conn);
}

/******************************************************************************/

FTPConnection::FTPConnection(const char * host) {
    if (!FtpConnect(host, &conn))
        throw FTPConnectException();
}

const char * FTPConnection::getLastResponse() {
    return FtpLastResponse(conn);
}

void FTPConnection::setConnectionMode(Mode mode) {
    FTPLIB(FtpOptions(FTPLIB_CONNMODE, mode, conn));
}

void FTPConnection::login(const char * user, const char * password) {
    FTPLIB(FtpLogin(user, password, conn));
}

std::string FTPConnection::getSystemType() {
    char buffer[BUFSIZE+1];
    FTPLIB(FtpSysType(buffer, BUFSIZE, conn));
    return buffer;
}

std::string FTPConnection::getDirectory() {
    char buffer[BUFSIZE+1];
    FTPLIB(FtpPwd(buffer, BUFSIZE, conn));
    return buffer;
}

void FTPConnection::cd(const char * directory) {
    FTPLIB(FtpChdir(directory, conn));
}

void FTPConnection::cdup() {
    FTPLIB(FtpCDUp(conn));
}

void FTPConnection::getList(const char * filename, const char * path) {
    FTPLIB(FtpNlst(filename, path, conn));
}

void FTPConnection::getFullList(const char * filename, const char * path) {
    FTPLIB(FtpDir(filename, path, conn));
}

void FTPConnection::mkdir(const char * directory) {
    FTPLIB(FtpMkdir(directory, conn));
}

void FTPConnection::rmdir(const char * directory) {
    FTPLIB(FtpRmdir(directory, conn));
}

void FTPConnection::rename(const char * oldName, const char * newName) {
    FTPLIB(FtpRename(oldName, newName, conn));
}

void FTPConnection::unlink(const char * filename) {
    FTPLIB(FtpDelete(filename, conn));
}

void FTPConnection::get(const char * local, const char * remote, TransferMode mode) {
    FTPLIB(FtpGet(local, remote, mode, conn));
}

void FTPConnection::put(const char * local, const char * remote, TransferMode mode) {
    FTPLIB(FtpPut(local, remote, mode, conn));
}

unsigned FTPConnection::size(const char * path, TransferMode mode) {
    unsigned result;
    FTPLIB(FtpSize(path, &result, mode, conn));
    return result;
}

std::string FTPConnection::modDate(const char * path) {
    char buffer[32];
    FTPLIB(FtpModDate(path, buffer, sizeof(buffer)-1, conn));
    return buffer;
}

void FTPConnection::site(const char * command) {
    FTPLIB(FtpSite(command, conn));
}

/******************************************************************************/

FTPDataConnection::FTPDataConnection(FTPConnection &connection,
        const char * path, Type type, TransferMode mode) {
    if (0==FtpAccess(path, type, mode, connection.conn, &conn))
        throw FTPException(connection.getLastResponse());
}

ssize_t FTPDataConnection::read(void * buffer, size_t length) {
    return FtpRead(buffer, length, conn);
}

ssize_t FTPDataConnection::write(void * buffer, size_t length) {
    return FtpWrite(buffer, length, conn);
}
    
/******************************************************************************/

__attribute__((constructor))
static void initialize() {
    FtpInit();
}
