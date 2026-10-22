/*
 * A381128  The number of card moves required to deal n cards using
 *          Down-SpellUnder dealing.
 * https://oeis.org/A381128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = A067278(n-1) + n where A067278(n-1) is the sum of the
 *            number of letters in the English spellings of the numbers 1
 *            through n-1 (no hyphens or "and"). For example, a(4) = (3+3+5) +
 *            4 = 15
 * Range:     u8 n <= 32, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Precomputed letter counts for units, teens and tens */
static const int ones_len[20] = {
    0,      /* 0 (unused) */
    3,      /* one     */
    3,      /* two     */
    5,      /* three   */
    4,      /* four    */
    4,      /* five    */
    3,      /* six     */
    5,      /* seven   */
    5,      /* eight   */
    4,      /* nine    */
    3,      /* ten     */
    6,      /* eleven  */
    6,      /* twelve  */
    8,      /* thirteen*/
    8,      /* fourteen*/
    7,      /* fifteen */
    7,      /* sixteen */
    9,      /* seventeen*/
    8,      /* eighteen */
    8       /* nineteen */
};

static const int tens_len[10] = {
    0, /* dummy for 0*10 */
    0, /* 10 is handled above */
    6, /* twenty */
    6, /* thirty */
    5, /* forty  */
    5, /* fifty  */
    5, /* sixty  */
    7, /* seventy */
    6, /* eighty */
    6  /* ninety */
};

/* Return the number of letters in the English spelling of x
   (0 < x <= 9999).  Uses the same convention as the OEIS example. */
static seq_t letter_count(idx_t x) {
    seq_t len = 0;
    if (x == 0) return 0;

    if (x >= 1000) {
        idx_t thousands = x / 1000;          /* 1..9 */
        /* "thousand" has 8 letters */
        len += (seq_t)ones_len[thousands] + (seq_t)8;
        x %= 1000;
        if (x == 0) return len;
    }

    if (x >= 100) {
        idx_t hundreds = x / 100;            /* 1..9 */
        /* "hundred" has 7 letters */
        len += (seq_t)ones_len[hundreds] + (seq_t)7;
        x %= 100;
        if (x == 0) return len;
    }

    if (x >= 20) {
        idx_t tens = x / 10;                 /* 2..9 */
        len += (seq_t)tens_len[tens];
        x %= 10;
        if (x == 0) return len;
    }

    /* 0 <= x < 20 */
    len += (seq_t)ones_len[x];
    return len;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i < n; ++i) {
        sum += letter_count(i);
    }
    return sum + n;
}
