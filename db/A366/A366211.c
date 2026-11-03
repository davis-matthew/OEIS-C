/*
 * A366211  Numbers whose median of the digits is equal to 5.
 * https://oeis.org/A366211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose median of the decimal digits is equal to 5
   (OEIS A366211).  This implementation searches for the
   n‑th number whose median digit equals 5, storing results
   in a static cache so that calls in any order are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Test whether the median of the decimal digits of x equals 5. */
static bool median_is_five(seq_t x)
{
    unsigned freq[10] = {0};
    seq_t t = x;

    /* Count the digits of x. */
    if (t == 0) {
        freq[0] = 1;
    } else {
        while (t != 0) {
            unsigned d = (unsigned)(t % 10);
            freq[d]++;
            t /= 10;
        }
    }

    /* Total number of digits. */
    int total = 0;
    for (int d = 0; d < 10; d++) total += freq[d];

    /* Odd number of digits: the middle digit after sorting must be 5. */
    if (total % 2 == 1) {
        int k = total / 2;          /* 0‑based index of middle digit */
        int cum = 0;
        for (int d = 0; d < 10; d++) {
            cum += freq[d];
            if (cum > k) {
                return d == 5;
            }
        }
    } else {  /* Even number of digits: the two middle digits must average 5. */
        int k = total / 2;          /* positions k-1 and k in 0‑based sorted order */
        int pos1 = k - 1;
        int pos2 = k;
        int cum = 0;
        int d1 = -1, d2 = -1;
        for (int d = 0; d < 10; d++) {
            cum += freq[d];
            if (d1 == -1 && cum > pos1) d1 = d;
            if (d2 == -1 && cum > pos2) { d2 = d; break; }
        }
        return (d1 != -1) && (d2 != -1) && (d1 + d2 == 10);
    }

    return false;
}

/* A large enough cache to hold thousands of terms. */
#define CACHE_SIZE 500000
static seq_t cache[CACHE_SIZE];
static size_t len = 0;   /* number of terms already found */
static seq_t cur = 5;    /* next integer to test for the median property */

/* Return the n-th number whose median of the decimal digits equals 5. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1, but guard anyway */

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        if (median_is_five(cur))
            cache[len++] = cur;
        cur++;
    }
    return cache[(size_t)n - 1];
}
