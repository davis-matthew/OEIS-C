/*
 * A362123  Number of letters in the British English name of n, excluding
 *          spaces and hyphens.
 * https://oeis.org/A362123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   Number of letters in the British English name of n, excluding
 *            spaces and hyphens
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses an iterative decomposition of the number into groups of thousands and
   a recursive helper for each 3‑digit chunk.  It also inserts the mandatory "and" that
   appears before the last two digits of any number that contains larger groups
   (e.g. “eight thousand and seventy seven”). */

#include <stdbool.h>

/* English word lengths for 0 – 19 */
static const seq_t len0to19[20] = {
    4,  /* zero   */
    3,  /* one    */
    3,  /* two    */
    5,  /* three  */
    4,  /* four   */
    4,  /* five   */
    3,  /* six    */
    5,  /* seven  */
    5,  /* eight  */
    4,  /* nine   */
    3,  /* ten    */
    6,  /* eleven */
    6,  /* twelve */
    8,  /* thirteen */
    8,  /* fourteen */
    7,  /* fifteen */
    7,  /* sixteen */
    9,  /* seventeen */
    8,  /* eighteen */
    8   /* nineteen */
};

/* Lengths of the tens (20, 30, …, 90) */
static const seq_t tens_len[10] = {
    0, /* unused */
    0, /* unused */
    6, /* twenty  */
    6, /* thirty  */
    5, /* forty   */
    5, /* fifty   */
    5, /* sixty   */
    7, /* seventy */
    6, /* eighty  */
    6  /* ninety  */
};

/* Powers of 1000 up to 10^15 (quadrillion). */
static const idx_t group_pow[5] = {
    1000ULL,                                 /* thousand     */
    1000ULL * 1000ULL,                       /* million      */
    1000ULL * 1000ULL * 1000ULL,             /* billion      */
    1000ULL * 1000ULL * 1000ULL * 1000ULL,   /* trillion     */
    1000ULL * 1000ULL * 1000ULL * 1000ULL * 1000ULL /* quadrillion */
};

/* Corresponding word lengths for the group names. */
static const seq_t group_len[5] = {
    8,  /* thousand  */
    7,  /* million   */
    7,  /* billion   */
    8,  /* trillion  */
    12 /* quadrillion */
};

/* Count letters for n > 0.  Recursive helper for each 3‑digit group. */
static seq_t count_letters_u64(idx_t n) {
    seq_t total = 0;
    bool has_group = false;

    /* Process larger groups (thousand, million, …). */
    for (int i = 4; i >= 0; --i) {
        if (n >= group_pow[i]) {
            idx_t part   = n / group_pow[i];
            n           %= group_pow[i];
            total      += count_letters_u64(part);
            total      += group_len[i];
            has_group = true;
        }
    }

    /* Insert the mandatory “and” before the last two digits if needed. */
    if (has_group && n > 0 && n < 100) {
        total += 3; /* “and” */
    }

    /* Handle the remaining < 1000. */
    if (n >= 100) {
        idx_t hundreds = n / 100;
        n             %= 100;
        total += len0to19[hundreds]; /* digits 1‑9 */
        total += 7;                  /* “hundred” */
        if (n > 0) {
            total += 3;              /* “and” */
        }
    }

    if (n >= 20) {
        idx_t tens = n / 10;
        n         %= 10;
        total += tens_len[tens];
    }
    if (n > 0 && n < 20) {
        total += len0to19[n];
    }

    return total;
}

/* Main sequence function.  a(0) = 4 for “zero”. */
seq_t a(idx_t n) {
    if (n == 0) return 4;            /* “zero” */
    return count_letters_u64(n);
}
