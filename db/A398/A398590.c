/*
 * A398590  Number of distinct entries k of the n X n multiplication table
 *          such that k + n + 1 + floor((k-1)/n) is an entry of the (n+1) X
 *          (n+1) multiplication table.
 * https://oeis.org/A398590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of distinct k in the n×n multiplication table such
 *            that k + n + 1 + floor((k-1)/n) is an element of the (n+1)×(n+1)
 *            multiplication table
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Set a bit in a bit‑set stored as an array of uint64_t. */
static inline void set_bit(uint64_t *bits, size_t idx)
{
    bits[idx >> 6] |= ((uint64_t)1 << (idx & 63));
}

/* Get a bit from a bit‑set. */
static inline int get_bit(const uint64_t *bits, size_t idx)
{
    return (bits[idx >> 6] >> (idx & 63)) & 1;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    const uint64_t N   = (uint64_t)n;      /* up to 64‑bit products are safe */
    const uint64_t N1  = N + 1;

    /* Bit‑set for P_n (products up to n×n).  Size in bits is (N*N)+1. */
    const size_t size_n  = (size_t)N * (size_t)N + 1;
    const size_t cap_n   = (size_n + 63) >> 6;
    uint64_t *bits_n = (uint64_t*)calloc(cap_n, sizeof(uint64_t));
    if (!bits_n) return (seq_t)0;          /* allocation failure */

    /* Bit‑set for P_{n+1}. */
    const size_t size_n1  = (size_t)N1 * (size_t)N1 + 1;
    const size_t cap_n1   = (size_n1 + 63) >> 6;
    uint64_t *bits_n1 = (uint64_t*)calloc(cap_n1, sizeof(uint64_t));
    if (!bits_n1) { free(bits_n); return (seq_t)0; }

    /* Populate P_{n+1}. */
    for (uint64_t i = 1; i <= N1; ++i) {
        for (uint64_t j = 1; j <= N1; ++j) {
            set_bit(bits_n1, i * j);
        }
    }

    /* Scan P_n, counting distinct products whose image lies in P_{n+1}. */
    seq_t count = 0;
    for (uint64_t i = 1; i <= N; ++i) {
        for (uint64_t j = 1; j <= N; ++j) {
            const uint64_t k = i * j;
            if (get_bit(bits_n, k))
                continue;           /* already seen */
            set_bit(bits_n, k);

            const uint64_t q  = (k - 1) / N;                 /* row offset */
            const uint64_t r  = ((k - 1) % N) + 1;          /* column (1‑based) */
            const uint64_t img = (q + 1) * N1 + r;          /* mapped value */

            /* img cannot exceed N1*N1, but guard anyway */
            if (img <= N1 * N1 && get_bit(bits_n1, img))
                ++count;
        }
    }

    free(bits_n);
    free(bits_n1);
    return count;
}
