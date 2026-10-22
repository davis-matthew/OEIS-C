/*
 * A398491  Number of distinct entries q of the n X n multiplication table
 *          such that floor(q/n) > floor(q/(n+1)).
 * https://oeis.org/A398491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all distinct products i*j (1 <= i <= j <= n)
   and counting those for which floor(i*j/n) > floor(i*j/(n+1)). */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard for safety */
    idx_t N = n;
    idx_t maxq = N * N;            /* maximum product */
    /* Number of bits we need (0 .. maxq inclusive) */
    idx_t bits = maxq + 1;
    size_t arrsz = (bits + 63) >> 6; /* number of uint64_t words */
    uint64_t *seen = (uint64_t *)malloc(arrsz * sizeof(uint64_t));
    if (!seen) return 0;           /* allocation failure; return 0 as fallback */
    memset(seen, 0, arrsz * sizeof(uint64_t));

    seq_t count = 0;
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; ++j) {   /* iterate only j >= i to avoid duplicate pairs */
            idx_t q = i * j;
            /* Check if we have already seen this product */
            size_t idx = q >> 6;             /* q / 64 */
            uint64_t mask = 1ULL << (q & 63);
            if (seen[idx] & mask) continue;  /* already counted */
            seen[idx] |= mask;

            /* Check the floor condition */
            idx_t fqn  = q / N;
            idx_t fqn1 = q / (N + 1);
            if (fqn > fqn1) count++;
        }
    }
    free(seen);
    return count;
}
