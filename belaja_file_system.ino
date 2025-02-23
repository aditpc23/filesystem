#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#define FORMAT_LITTLEFS_IF_FAILED true

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  //Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory (null file)");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("- failed to open directory (not a directory)");
    return;
  }

  int fileCounter = 0;
  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      fileCounter++;
      Serial.printf(" %d. %s\t\n", fileCounter, file.name());
    }
    file = root.openNextFile();
  }
}


void createDir(fs::FS &fs, const char *path) {  //createdir berfungsi untuk membuat folder file
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {  //berfungsi untuk menghapus folder
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

void testFileIO(fs::FS &fs, const char *path) {
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing");
  uint32_t start = millis();
  for (i = 0; i < 2048; i++) {
    if ((i & 0x001F) == 0x001F) {
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %lu ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if (file && !file.isDirectory()) {
    len = file.size();
    size_t flen = len;
    start = millis();
    Serial.print("- reading");
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F) {
        Serial.print(".");
      }
      len -= toRead;
    }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %lu ms\r\n", flen, end);
    file.close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}

void setup() {
  Serial.begin(115200);

  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  //Serial.println("SPIFFS-like write file to new path and delete it w/folders");
  // writeFile(LittleFS, "/relay_states.txt", "1,0,1,0,1,0,1,0");
  // writeFile(LittleFS, "/hello2.txt", "Hello2");
  // writeFile(LittleFS, "/hello1.txt", "Hello1");

  //listDir(LittleFS, "/", 3);
  //deleteFile2(LittleFS, "/mydir/hello2.txt");

  //listDir(LittleFS, "/playlist", 3);
  //createDir(LittleFS, "/mydir");
  //writeFile(LittleFS, "/mydir/hello2.txt", "Hello2");
  //listDir(LittleFS, "/", 1);
  // removeDir(LittleFS, "/mydir");
  // listDir(LittleFS, "/", 1);
  // writeFile(LittleFS, "/hello.txt", "Hello ");
  // appendFile(LittleFS, "/hello.txt", "World!\r\n");
  // Serial.println("readfile");

  // readFile(LittleFS, "/hello2.txt");
  // readFile(LittleFS, "/hello1.txt");

  // readFile(LittleFS, "/new1/new2/new3/hello3.txt");
  // renameFile(LittleFS, "/hello.txt", "/foo.txt");
  // readFile(LittleFS, "/foo.txt");
  // deleteFile(LittleFS, "/foo.txt");
  // testFileIO(LittleFS, "/test.txt");
  // deleteFile(LittleFS, "/test.txt");
  //Serial.println("Test complete");
  readFile(LittleFS, "/myplaylist/0,0,0,0,0,1,0,0,0,0,0,0,138");
  //listDir(LittleFS, "/myplaylist",1);
}

void loop() {
  //listDir(LittleFS, "/myplaylist",1);
  
  //createDir(LittleFS, "/myplaylist");
  //delay(2000);
  //deleteFile(LittleFS,"/history.txt");

  // File file = LittleFS.open("/myplaylist/animasi2", "w");
  // if (file) {
  //   //file.println("1,0,0,0,0,0,0,0");
  //   //file.println("0,1,0,0,0,0,0,0");
  //   file.println("1,0,0,0,0,0,0,0");
  //   file.println("0,1,0,0,0,0,0,0");
  //   file.println("0,0,1,0,0,0,0,0");
  //   file.println("0,0,0,1,0,0,0,0");
  //   file.println("0,0,0,0,1,0,0,0");
  //   file.println("0,0,0,0,0,1,0,0");
  //   file.println("0,0,0,0,0,0,1,0");
  //   file.println("0,0,0,0,0,0,0,1");
  //   file.println("0,0,0,0,0,0,1,0");
  //   file.println("0,0,0,0,0,1,0,0");
  //   file.println("0,0,0,0,1,0,0,0");
  //  // file.println("0,1,0,0,0,0,0,0");
  //  // file.println("1,0,0,0,0,0,0,0");
  //   // file.println("0,0,0,0,0,0,0,1");
  //   // file.println("0,0,0,0,0,0,0,0");

  //   file.close();
  //   Serial.println("SUccess");
  // } else {
  //   Serial.println("Failed to create relay_states.txt file");
  // }

  //delay(1000);
}