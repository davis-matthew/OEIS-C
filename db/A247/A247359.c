/*
 * A247359  Numbers whose English name ends in a consonant.
 * https://oeis.org/A247359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth integer whose English name ends in a consonant (A247359).
   This is a search-based implementation. */
#include <stdbool.h>

/* Whether the English name of a number <20 ends in a consonant.      */
static const bool units_cons[20] = {
    false,          /* 0 unused */
    false,          /* 1 one    ends e */
    false,          /* 2 two    ends o */
    false,          /* 3 three  ends e */
    true,           /* 4 four   ends r */
    false,          /* 5 five   ends e */
    true,           /* 6 six    ends x */
    true,           /* 7 seven  ends n */
    true,           /* 8 eight  ends t */
    false,          /* 9 nine   ends e */
    true,           /*10 ten    ends n */
    true,           /*11 eleven ends n */
    false,          /*12 twelve ends e */
    true,           /*13 thirteen ends n */
    true,           /*14 fourteen ends n */
    true,           /*15 fifteen ends n */
    true,           /*16 sixteen ends n */
    true,           /*17 seventeen ends n */
    true,           /*18 eighteen ends n */
    true            /*19 nineteen ends n */
};

/* Whether the English name of a tens value (20,30,...,90) ends in a consonant */
static const bool tens_cons[9] = {
    true, /* twenty  */
    true, /* thirty  */
    true, /* forty   */
    true, /* fifty   */
    true, /* sixty   */
    true, /* seventy */
    true, /* eighty  */
    true, /* ninety  */
};

/* Return true if the English name of n ends in a consonant. */
static bool is_cons(idx_t n) {
    if (n < 20)
        return units_cons[n];

    if (n < 100) {
        if (n % 10 == 0)
            return tens_cons[n / 10 - 2];
        return units_cons[n % 10];
    }

    if (n < 1000) {
        if (n % 100 == 0)          /* 100,200,...,900 */
            return true;           /* "hundred" ends with 'd' */
        return is_cons(n % 100);    /* rest after "hundred" */
    }

    if (n < 1000000) {             /* 1,000 to 999,999 */
        if (n % 1000 == 0)         /* 1000,2000,... */
            return true;           /* "thousand" ends with 'd' */
        return is_cons(n % 1000);  /* rest after "thousand" */
    }

    if (n < 1000000000) {          /* 1,000,000 to 999,999,999 */
        if (n % 1000000 == 0)     /* 1,000,000,2,000,000,... */
            return true;           /* "million" ends with 'n' */
        return is_cons(n % 1000000);
    }

    /* For larger numbers the pattern repeats with the next higher power. */
    if (n % 1000000000 == 0)       /* 1,000,000,000 ... */
        return true;               /* "billion" ends with 'n' */
    return is_cons(n % 1000000000);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t num = 0;
    while (1) {
        ++num;
        if (is_cons(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
