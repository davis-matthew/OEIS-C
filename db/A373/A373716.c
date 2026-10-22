/*
 * A373716  a(n) is the number of distinct products i*j minus the number of
 *          distinct sums i+j with 1 <= i, j <= n.
 * https://oeis.org/A373716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct products i*j (1≤i,j≤n) minus * the
 *            number of distinct sums i+j (2≤i,j≤n). * a(n) = A027424(n) -
 *            (2n-1). * * Implementation: Enumerate all products i*j for
 *            1≤i,j≤n and mark * the reached integers in a bitset. After the
 *            enumeration the number * of set bits gives the number...
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Assume seq_t and idx_t are defined elsewhere in the build. */
seq_t a(idx_t n)
{
    if (n <= 1) {
        /* For n = 0 or 1 the answer is 0. */
        return (seq_t)0;
    }

    /* Size of the bitset to hold integers 1..n*n. */
    size_t bitset_len = ((size_t)n * (size_t)n + 7) >> 3;
    unsigned char *bits = (unsigned char*)calloc(bitset_len, 1);
    if (!bits) return (seq_t)-1;  /* Out‑of‑memory fallback */

    /* Mark all products i*j. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            idx_t prod = i * j;          /* 1 ≤ prod ≤ n² */
            size_t idx  = (size_t)prod - 1;
            bits[idx >> 3] |= (unsigned char)(1U << (idx & 7));
        }
    }

    /* Count set bits (distinct products). */
    size_t distinct = 0;
    for (size_t k = 0; k < bitset_len; ++k) {
        distinct += __builtin_popcount((unsigned)bits[k]);
    }
    free(bits);

    seq_t prod_count = (seq_t)distinct;
    seq_t sums      = (seq_t)2 * (seq_t)n - (seq_t)1;
    return prod_count - sums;
}
