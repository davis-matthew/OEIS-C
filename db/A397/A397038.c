/*
 * A397038  Sum of the quotients floor(k/n) over the distinct entries k of the
 *          n X n multiplication table.
 * https://oeis.org/A397038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k in {i*j : 1≤i,j≤n}} floor(k / n)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the set of distinct products using a bitset
   and accumulates the required sum. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for out‑of‑range calls */

    /* Number of bits needed to represent products up to n^2. */
    idx_t N2   = n * n;                     /* fits in 64 bits for n ≤ 10000 */
    idx_t words = (N2 >> 6) + 1;             /* divide by 64 and round up */

    /* Allocate and clear bitmask. */
    uint64_t *mask = (uint64_t*)calloc((size_t)words, sizeof(uint64_t));
    if (!mask) return 0;                    /* allocation failure */

    /* Mark all distinct products i*j (with i ≤ j to avoid double counting). */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            idx_t prod = i * j;              /* product fits in idx_t */
            uint64_t *p = mask + (prod >> 6);
            *p |= 1ULL << (prod & 63UL);
        }
    }

    /* Sum floor(k / n) over all set bits (distinct products). */
    seq_t sum = 0;
    for (idx_t w = 0; w < words; ++w) {
        uint64_t bits = mask[w];
        while (bits) {
            unsigned off = __builtin_ctzll(bits);  /* lowest set bit */
            idx_t k = (w << 6) + off;             /* actual product value */
            sum += (seq_t)k / (seq_t)n;
            bits &= bits - 1;                     /* clear lowest bit */
        }
    }

    free(mask);
    return sum;
}
