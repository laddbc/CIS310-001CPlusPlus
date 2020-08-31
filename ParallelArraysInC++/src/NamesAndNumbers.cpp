#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;
// ====================================================================
// Declare _global_ variables
// ====================================================================
//   (would be better as fields in a class...)
const int MAXIMUM_NUMBER_OF_BOOKS = 15; // the max book count
int bookCnt = 0;                        // current book count
double prices[MAXIMUM_NUMBER_OF_BOOKS]; // the prices
char * titles[MAXIMUM_NUMBER_OF_BOOKS]; // the titles



/**
 * Is the input redirected?
 *
 * @return true if input is redirected.
 */
bool isInputRedirected() {
  return !isatty(STDIN_FILENO);
}

/**
 * Add the given book to the array of books if possible.
 *
 * @param price price of the book to add
 * @param title title of the book to add; will be kept by array
 * @return true if it was added, false otherwise
 * @note bookCnt will be incremented and prices/titles
 *       change on success
 */
bool addABook(double price, char * title) {
  if (bookCnt < MAXIMUM_NUMBER_OF_BOOKS) {
    prices[bookCnt] = price;
    titles[bookCnt] = title;
    bookCnt++;
    return true;
  }
  return false;
}

/**
 * Print the contents of the prices/titles arrays, one per line.
 */
void printAllBooks() {
  for (int i = 0; i < bookCnt; ++i) {
    printf("$%5.2f %s\n", prices[i], titles[i]);
  }
}

/**
 * Is the array full?
 *
 * @return true if there are no remaining slots
 */
bool isArrayFull() {
  return bookCnt == MAXIMUM_NUMBER_OF_BOOKS;
}

/**
 * Return pointer to first character NOT matching ch OR pointing at '\0'.
 *
 * @param str string to search; cannot be NULL!
 * @param ch character to unmatch
 */
char * unMatch(char * str, char ch) {
  while (*str == ch && *str != '\0') str++;
  return str;
}

/**
 * Return pointer to first character matching ch OR pointing at '\0'.
 *
 * @param str string to search; cannot be NULL!
 * @param ch character to match
 */
char * match(char * str, char ch) {
  while (*str != ch && *str != '\0') str++;
  return str;
}

/**
 * Prompt the user for prices and titles. Fills prices/titles from
 * bookCnt forward (does not empty the parallel arrays). Does not read past
 * MAXIMUM_NUMBER_OF_BOOKS entries.
 */
void fillTheArrays() {
  for(bool done = false;
      !done;
      done = done || isArrayFull()) {
    // Input line from the user
    char line[80];
    // Copy of the "price" portion of the line (might be whole line)
    char number[80];

    // pointer into line; after leading spaces
    char * firstNonBlank = nullptr;
    // pointer into line; first space after firstNonBlank
    char * nextBlank = nullptr;

    printf("Enter price, space, and the title. Type \"done\" when done. ");
    // read a whole line *including* the EOLN
    done = (fgets(line, 80, stdin) == NULL);

    if (done)
      continue;

    // echo input if input is redirected from a file
    if (isInputRedirected())
      printf("%s", line);

    line[strlen(line) - 1] = '\0'; // replace EOLN with NULL (make line shorter)
    firstNonBlank = unMatch(line, ' ');
    if (*firstNonBlank == '\0')
      continue; // line contains only spaces

    nextBlank = match(firstNonBlank, ' ');

    int lengthOfPrice = nextBlank - firstNonBlank;
    strncpy(number, firstNonBlank, lengthOfPrice);
    number[lengthOfPrice] = '\0';

    // check if we're done
    if (strcmp(number, "done") == 0) {
      done = true;
      continue;
    }
      // attempt to convert price
      double price = atof(number);

      if (price <= 0.0) {
        printf("Error: price '%s' could not be interpreted; line ignored.\n", number);
        continue;
      }

      // check the title
      if (*nextBlank == '\0' || // no blank so no title
          ((nextBlank - firstNonBlank) == 80) || // at end of buffer so no title
          *(nextBlank +1) == '\0') { // empty after single space so no title
        printf("Error: no title provided; line ignored.\n");
        continue;
      }

      nextBlank++; // move past blank

      int titleLengthWithNull = strlen(nextBlank) + 1;
      char * title = new char[titleLengthWithNull];
      strcpy(title, nextBlank);
      addABook(price, title);
  }
}

int main(int argc, char *argv[]) {
  fillTheArrays();
  printAllBooks();
  return 0;
}
