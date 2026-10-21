/*
 * A260152  a(n) is the number of distinct sums s + t where s, t are divisors
 *          of n.
 * https://oeis.org/A260152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums s+t with s, t | n
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates the divisors of n, then
   marks all pairwise sums using a boolean array.  The result
   is the count of distinct entries in that array. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* gather divisors of n */
    size_t cap = 32;
    seq_t *divs = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!divs) return 0;          /* allocation failure: return 0 */
    size_t divcnt = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            if (divcnt == cap) { /* grow array */
                cap <<= 1;
                divs = (seq_t*)realloc(divs, cap * sizeof(seq_t));
                if (!divs) return 0;
            }
            divs[divcnt++] = (seq_t)i;

            idx_t j = n / i;
            if (j != i) {
                if (divcnt == cap) {
                    cap <<= 1;
                    divs = (seq_t*)realloc(divs, cap * sizeof(seq_t));
                    if (!divs) return 0;
                }
                divs[divcnt++] = (seq_t)j;
            }
        }
    }

    /* max possible sum is 2*n, we allocate an array of that size plus
       one for the zero index (unused). */
    if (n > (UINT64_MAX >> 1)) {
        free(divs);
        return 0;  /* too large to handle safely */
    }

    idx_t maxsum = n * 2;
    uint8_t *seen = (uint8_t*)calloc((size_t)maxsum + 1, sizeof(uint8_t));
    if (!seen) {
        free(divs);
        return 0;  /* allocation failure */
    }

    seq_t count = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        for (size_t j = 0; j < divcnt; ++j) {
            seq_t sum = divs[i] + divs[j];
            idx_t idx = (idx_t)sum;  /* sum fits in idx_t as sum <= 2*n */
            if (!seen[idx]) {
                seen[idx] = 1;
                ++count;
            }
        }
    }

    free(seen);
    free(divs);
    return count;
}
