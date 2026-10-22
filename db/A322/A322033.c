/*
 * A322033  Numbers which when written in German do not contain the letter
 *          "e".
 * https://oeis.org/A322033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A322033: numbers whose German spelling contains no letter "e" */
/* Implementation: table of valid numbers (0 to 99).  For n > 13 return 0. */
#include <stddef.h>

static const seq_t valid_terms[] = {
    0, 5, 8, 12, 20, 25, 28, 50, 55, 58, 80, 85, 88
};
static const size_t valid_len = sizeof valid_terms / sizeof valid_terms[0];

seq_t a(idx_t n) {
    if (n == 0) return 0;                          /* undefined for n=0 */
    if (n <= (idx_t)valid_len) return valid_terms[n - 1];
    return 0;                                      /* beyond the finite list */
}
