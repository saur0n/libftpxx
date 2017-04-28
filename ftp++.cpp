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
        throw Exception(getLastResponse());

using namespace ftp;

ConnectionBase::~ConnectionBase() {
    FtpClose(conn);
}

/******************************************************************************/

Connection::Connection(const char * host) {
    if (!FtpConnect(host, &conn))
        throw ConnectException();
}

const char * Connection::getLastResponse() {
    return FtpLastResponse(conn);
}

void Connection::setConnectionMode(Mode mode) {
    FTPLIB(FtpOptions(FTPLIB_CONNMODE, mode, conn));
}

void Connection::login(const char * user, const char * password) {
    FTPLIB(FtpLogin(user, password, conn));
}

std::string Connection::getSystemType() {
    char buffer[BUFSIZE+1];
    FTPLIB(FtpSysType(buffer, BUFSIZE, conn));
    return buffer;
}

std::string Connection::getDirectory() {
    char buffer[BUFSIZE+1];
    FTPLIB(FtpPwd(buffer, BUFSIZE, conn));
    return buffer;
}

void Connection::cd(const char * directory) {
    FTPLIB(FtpChdir(directory, conn));
}

void Connection::cdup() {
    FTPLIB(FtpCDUp(conn));
}

void Connection::getList(const char * filename, const char * path) {
    FTPLIB(FtpNlst(filename, path, conn));
}

void Connection::getFullList(const char * filename, const char * path) {
    FTPLIB(FtpDir(filename, path, conn));
}

void Connection::mkdir(const char * directory) {
    FTPLIB(FtpMkdir(directory, conn));
}

void Connection::rmdir(const char * directory) {
    FTPLIB(FtpRmdir(directory, conn));
}

void Connection::rename(const char * oldName, const char * newName) {
    FTPLIB(FtpRename(oldName, newName, conn));
}

void Connection::unlink(const char * filename) {
    FTPLIB(FtpDelete(filename, conn));
}

void Connection::get(const char * local, const char * remote, TransferMode mode) {
    FTPLIB(FtpGet(local, remote, mode, conn));
}

void Connection::put(const char * local, const char * remote, TransferMode mode) {
    FTPLIB(FtpPut(local, remote, mode, conn));
}

unsigned Connection::size(const char * path, TransferMode mode) {
    unsigned result;
    FTPLIB(FtpSize(path, &result, mode, conn));
    return result;
}

std::string Connection::modDate(const char * path) {
    char buffer[32];
    FTPLIB(FtpModDate(path, buffer, sizeof(buffer)-1, conn));
    return buffer;
}

void Connection::site(const char * command) {
    FTPLIB(FtpSite(command, conn));
}

/******************************************************************************/

DataConnection::DataConnection(Connection &connection,
        const char * path, Type type, TransferMode mode) {
    if (0==FtpAccess(path, type, mode, connection.conn, &conn))
        throw Exception(connection.getLastResponse());
}

ssize_t DataConnection::read(void * buffer, size_t length) {
    return FtpRead(buffer, length, conn);
}

ssize_t DataConnection::write(void * buffer, size_t length) {
    return FtpWrite(buffer, length, conn);
}
    
/******************************************************************************/

__attribute__((constructor))
static void initialize() {
    FtpInit();
}
