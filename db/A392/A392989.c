/*
 * A392989  Number of character comparisons needed by naive string search to
 *          identify the first occurrence of string n in the infinite
 *          Champernowne word 123456789101112... (A033307).
 * https://oeis.org/A392989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of character comparisons performed by the * naive
 *            string search for the decimal representation of n * in the
 *            Champernowne word 123456789101112
 * Range:     u8 n <= 108, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* convert an unsigned integer to its decimal string representation.
 * buf must be large enough to hold the digits (up to 20 for 64‑bit
 * numbers).  *len receives the number of digits. */
static void digits_of_uint64(uint64_t x, char *buf, size_t *len) {
    if (x == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[32];
    size_t d = 0;
    while (x > 0) {
        tmp[d++] = '0' + (x % 10);
        x /= 10;
    }
    for (size_t i = 0; i < d; ++i)
        buf[i] = tmp[d - 1 - i];
    *len = d;
}

/* number of character comparisons needed by the naive string search
 * to identify the first occurrence of the decimal representation of n
 * in the infinite Champernowne word 123456789101112... */
seq_t a(idx_t n) {
    /* pattern = decimal representation of n */
    char pattern[32];
    size_t plen;
    digits_of_uint64((uint64_t)n, pattern, &plen);

    /* build a sufficient prefix of the Champernowne word */
    /* for n <= 10000 the first occurrence lies before
     * about 40000 digits; 500000 is far more than enough. */
    static char cham[500000];
    size_t clen = 0;
    for (uint64_t num = 1; clen < sizeof(cham); ++num) {
        char tmp[32];
        size_t d;
        digits_of_uint64(num, tmp, &d);
        for (size_t i = 0; i < d && clen < sizeof(cham); ++i) {
            cham[clen++] = tmp[i];
        }
    }

    /* naive string search, counting comparisons */
    seq_t comparisons = 0;
    for (size_t i = 0; i + plen <= clen; ++i) {
        size_t j;
        for (j = 0; j < plen; ++j) {
            comparisons++;                /* count this comparison */
            if (cham[i + j] != pattern[j])
                break;                    /* mismatch */
        }
        if (j == plen)                    /* full match */
            return comparisons;
    }

    /* Should never reach here for the defined range of n */
    return comparisons;
}
