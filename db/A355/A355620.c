/*
 * A355620  a(n) is the sum of the divisors of n whose decimal expansions
 *          appear as substrings in the decimal expansion of n.
 * https://oeis.org/A355620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the divisors d of n such that the decimal
 *            representation of d is a substring of the decimal representation
 *            of n
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: n is indexed from 1; a(0) is defined as 0 for safety. */
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, undefined for 0 */

    /* Convert n once to a string for substring tests. */
    char nbuf[64];
    size_t nlen = 0;
    seq_to_str((seq_t)n, nbuf, &nlen);
    nbuf[nlen] = '\0';

    seq_t sum = 0;

    /* Enumerate all divisors via d <= n/d to avoid overflow. */
    idx_t d;
    for (d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* Check divisor d. */
        char dbuf[64];
        size_t dlen = 0;
        seq_to_str((seq_t)d, dbuf, &dlen);
        dbuf[dlen] = '\0';
        if (strstr(nbuf, dbuf) != NULL) sum += (seq_t)d;

        /* Complementary divisor d2 = n/d (avoiding double counting). */
        idx_t d2 = n / d;
        if (d2 != d) {
            char dbuf2[64];
            size_t d2len = 0;
            seq_to_str((seq_t)d2, dbuf2, &d2len);
            dbuf2[d2len] = '\0';
            if (strstr(nbuf, dbuf2) != NULL) sum += (seq_t)d2;
        }
    }

    return sum;
}
