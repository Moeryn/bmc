/*
 * BRLTTY - A background process providing access to the console screen (when in
 *          text mode) for a blind person using a refreshable braille display.
 *
 * Copyright (C) 1995-2011 by The BRLTTY Developers.
 *
 * BRLTTY comes with ABSOLUTELY NO WARRANTY.
 *
 * This is free software, placed under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version. Please see the file LICENSE-GPL for details.
 *
 * Web Page: http://mielke.cc/brltty/
 *
 * This software is maintained by Dave Mielke <dave@mielke.cc>.
 */

#include "prologue.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif /* HAVE_SYS_FILE_H */

#include "file.h"
#include "log.h"

static char *
joinStrings (const char *const *strings, int count) {
  char *string;
  size_t length = 0;
  size_t lengths[count];
  int index;

  for (index=0; index<count; index+=1) {
    length += lengths[index] = strlen(strings[index]);
  }

  if ((string = malloc(length+1))) {
    char *target = string;

    for (index=0; index<count; index+=1) {
      length = lengths[index];
      memcpy(target, strings[index], length);
      target += length;
    }

    *target = 0;
  }

  return string;
}

int
isPathDelimiter (const char character) {
  return character == '/';
}

int
isAbsolutePath (const char *path) {
  if (isPathDelimiter(path[0])) return 1;

#if defined(__MINGW32__) || defined(__MSDOS__)
  if (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", toupper(path[0])) && (path[1] == ':')) return 1;
#endif /* defined(__MINGW32__) || defined(__MSDOS__) */

  return 0;
}

static size_t
stripPathDelimiter (const char *path, size_t length) {
  while (length) {
    if (!isPathDelimiter(path[length-1])) break;
    length -= 1;
  }
  return length;
}

char *
getPathDirectory (const char *path) {
  size_t length = strlen(path);
  size_t end = stripPathDelimiter(path, length);

  if (end) {
    while (--end)
      if (isPathDelimiter(path[end-1]))
        break;

    if ((length = end))
      if ((end = stripPathDelimiter(path, length)))
        length = end;
  }

  if (!length) length = strlen(path = ".");
  {
    char *directory = malloc(length + 1);
    if (directory) {
      memcpy(directory, path, length);
      directory[length] = 0;
    }
    return directory;
  }
}

const char *
locatePathName (const char *path) {
  const char *name = path + strlen(path);

  while (name != path) {
    if (isPathDelimiter(*--name)) {
      ++name;
      break;
    }
  }

  return name;
}

const char *
locatePathExtension (const char *path) {
  const char *name = locatePathName(path);
  return strrchr(name, '.');
}

int
isExplicitPath (const char *path) {
  return locatePathName(path) != path;
}

char *
makePath (const char *directory, const char *file) {
  const int count = 3;
  const char *strings[count];
  int first = count;

  strings[--first] = file;
  if (!isAbsolutePath(file)) {
    if (directory && *directory) {
      if (!isPathDelimiter(directory[strlen(directory)-1])) strings[--first] = "/";
      strings[--first] = directory;
    }
  }

  return joinStrings(&strings[first], count-first);
}

char *
ensureExtension (const char *path, const char *extension) {
  const char *strings[2];
  int count = 0;
  const size_t pathLength = strlen(path);
  const size_t extensionLength = strlen(extension);

  strings[count++] = path;
  if (extensionLength)
    if ((pathLength < extensionLength) ||
        (strcmp(&path[pathLength-extensionLength], extension) != 0))
      strings[count++] = extension;

  return joinStrings(strings, count);
}

int
testPath (const char *path) {
  return access(path, F_OK) != -1;
}

int
testFilePath (const char *path) {
#ifdef S_ISREG
  struct stat status;

  if (stat(path, &status) != -1) {
    if (S_ISREG(status.st_mode)) return 1;
    errno = EEXIST;
  }
#else /* S_ISREG */
  int result = open(path, O_RDONLY);

  if (result != -1) {
    close(result);
    return 1;
  }
#endif /* S_ISREG */

  return 0;
}

int
ensureDirectory (const char *path) {
  struct stat status;
  if (stat(path, &status) != -1) {
    if (S_ISDIR(status.st_mode)) return 1;
    logMessage(LOG_ERR, "not a directory: %s", path);
  } else if (errno != ENOENT) {
    logMessage(LOG_ERR, "cannot access directory: %s: %s", path, strerror(errno));
  } else if (mkdir(path
#ifndef __MINGW32__
                  ,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH
#endif /* __MINGW32__ */
                  ) != -1) {
    logMessage(LOG_NOTICE, "directory created: %s", path);
    return 1;
  } else {
    logMessage(((errno == ENOENT)? LOG_DEBUG: LOG_WARNING),
               "cannot create directory: %s: %s", path, strerror(errno));
  }
  return 0;
}

char *
getWorkingDirectory (void) {
  size_t size = 0X80;
  char *buffer = NULL;

  do {
    {
      char *newBuffer = realloc(buffer, size<<=1);
      if (!newBuffer) break;
      buffer = newBuffer;
    }

    if (getcwd(buffer, size)) return buffer;
  } while (errno == ERANGE);

  if (buffer) free(buffer);
  return NULL;
}

char *
getHomeDirectory (void) {
  char *path = getenv("HOME");
  if (!path || !*path) return NULL;
  return strdup(path);
}

char *
getOverrideDirectory (void) {
  static const char subdirectory[] = "." PACKAGE_NAME;

  {
    char *homeDirectory = getHomeDirectory();
    if (homeDirectory) {
      char *directory = makePath(homeDirectory, subdirectory);
      free(homeDirectory);
      if (directory) return directory;
    }
  }

  {
    char *workingDirectory = getWorkingDirectory();
    if (workingDirectory) {
      char *directory = makePath(workingDirectory, subdirectory);
      free(workingDirectory);
      if (directory) return directory;
    }
  }

  return NULL;
}

FILE *
openFile (const char *path, const char *mode, int optional) {
  FILE *file = fopen(path, mode);
  if (file) {
    logMessage(LOG_DEBUG, "file opened: %s fd=%d", path, fileno(file));
  } else {
    logMessage((optional && (errno == ENOENT))? LOG_DEBUG: LOG_ERR,
               "cannot open file: %s: %s", path, strerror(errno));
  }
  return file;
}

FILE *
openDataFile (const char *path, const char *mode, int optional) {
  static const char *overrideDirectory = NULL;
  const char *name = locatePathName(path);
  char *overridePath;
  FILE *file;

  if (!overrideDirectory) {
    if ((overrideDirectory = getOverrideDirectory())) {
      logMessage(LOG_DEBUG, "override directory: %s", overrideDirectory);
    } else {
      logMessage(LOG_DEBUG, "no override directory");
      overrideDirectory = "";
    }
  }

  if (!*overrideDirectory) {
    overridePath = NULL;
  } else if ((overridePath = makePath(overrideDirectory, name))) {
    if (testPath(overridePath)) {
      file = openFile(overridePath, mode, optional);
      goto done;
    }
  }

  if (!(file = openFile(path, mode, optional))) {
    if (((*mode == 'w') || (*mode == 'a')) && (errno == EACCES) && overridePath) {
      if (ensureDirectory(overrideDirectory)) file = openFile(overridePath, mode, optional);
    }
  }

done:
  if (overridePath) free(overridePath);
  return file;
}

int
readLine (FILE *file, char **buffer, size_t *size) {
  char *line;

  if (ferror(file)) return 0;
  if (feof(file)) return 0;
  if (!*size) *buffer = malloc(*size = 0X80);

  if ((line = fgets(*buffer, *size, file))) {
    size_t length = strlen(line); /* Line length including new-line. */

    /* No trailing new-line means that the buffer isn't big enough. */
    while (line[length-1] != '\n') {
      /* If necessary, extend the buffer. */
      if ((*size - (length + 1)) == 0)
        *buffer = realloc(*buffer, (*size <<= 1));

      /* Read the rest of the line into the end of the buffer. */
      if (!(line = fgets(&(*buffer)[length], *size-length, file))) {
        if (!ferror(file)) return 1;
        logSystemError("read");
        return 0;
      }

      length += strlen(line); /* New total line length. */
      line = *buffer; /* Point to the beginning of the line. */
    }

    if (--length > 0)
      if (line[length-1] == '\r')
        --length;
    line[length] = 0; /* Remove trailing new-line. */
    return 1;
  } else if (ferror(file)) {
    logSystemError("read");
  }

  return 0;
}

/* Process each line of an input text file safely.
 * This routine handles the actual reading of the file,
 * insuring that the input buffer is always big enough,
 * and calls a caller-supplied handler once for each line in the file.
 * The caller-supplied data pointer is passed straight through to the handler.
 */
int
processLines (FILE *file, LineHandler handleLine, void *data) {
  unsigned int lineNumber = 0;
  char *buffer = NULL;
  size_t bufferSize = 0;

  while (readLine(file, &buffer, &bufferSize)) {
    char *line = buffer;

    if (!lineNumber++) {
      static const char utf8ByteOrderMark[] = {0XEF, 0XBB, 0XBF};
      static const unsigned int length = sizeof(utf8ByteOrderMark);
      if (strncmp(line, utf8ByteOrderMark, length) == 0) line += length;
    }

    if (!handleLine(line, data)) break;
  }

  if (buffer) free(buffer);
  return !ferror(file);
}

void
formatInputError (char *buffer, int size, const char *file, const int *line, const char *format, va_list argp) {
  int length = 0;

  if (file) {
    snprintf(&buffer[length], size-length, "%s", file);
    length += strlen(&buffer[length]);
  }

  if (line) {
    snprintf(&buffer[length], size-length, "[%d]", *line);
    length += strlen(&buffer[length]);
  }

  if (length) {
    snprintf(&buffer[length], size-length, ": ");
    length += strlen(&buffer[length]);
  }

  vsnprintf(&buffer[length], size-length, format, argp);
}
