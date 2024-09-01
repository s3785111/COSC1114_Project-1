#include "pathcat.h"

/* =============================================================================== */
/**
 * @brief Appends two strings into a destination buffer with validation.
 *
 * @param dest   Destination buffer to hold the concatenated result.
 * @param a      First string to append.
 * @param b      Second string to append.
 * @return       0 on success, 1 on error.
 **
 * =============================================================================== */

int pathcat(char *dest, char *a, char *b) {
  size_t len_a         = strlen(a);
  size_t len_b         = strlen(b);
  size_t needed_length = len_a + len_b + 1; // +1 for the '/'

  // Check if the combined length fits within the destination buffer
  if (needed_length >= FILENAME_MAX) {
    fprintf(stderr, "Path Error: Concatenated path exceeds limit.\n");
    return 1;
  }

  // Copy and format path strings
  snprintf(dest, FILENAME_MAX, "%s", a);               // Copy the first string into dest
  strncat(dest, "/", FILENAME_MAX - strlen(dest) - 1); // Append path delimiter
  strncat(dest, b, FILENAME_MAX - strlen(dest) - 1);   // Append second string

#ifdef DEBUG
  printf("Concatenated %s\n", dest);
#endif

  return 0;
}
