/*
 * A320847  Length of the longest common subsequence of the first n terms of
 *          the Thue-Morse sequence (A010060) and its bitwise complement.
 * https://oeis.org/A320847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest common subsequence between * the
 *            first n terms of the Thue‑Morse sequence and its bitwise
 *            complement
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   bitset_or, bitset_shift_left, bitset_subtract
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bitset_or.h"
#include "../utils/bitset_shift_left.h"
#include "../utils/bitset_subtract.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Number of 64‑bit words needed to store n bits. */
    size_t W = (n + 63) >> 6;

    /* Allocate bit masks for positions of 0 and 1 in the Thue‑Morse sequence. */
    uint64_t *M0 = (uint64_t*)calloc(W, sizeof(uint64_t));
    uint64_t *M1 = (uint64_t*)calloc(W, sizeof(uint64_t));
    if (!M0 || !M1) return (seq_t)0;   /* allocation failure – return 0 (unlikely) */

    /* Build masks. */
    for (idx_t i = 0; i < n; ++i) {
        unsigned parity = __builtin_parityll((unsigned long long)i);
        if (parity == 0)
            M0[(size_t)(i >> 6)] |= 1ULL << (i & 63);
        else
            M1[(size_t)(i >> 6)] |= 1ULL << (i & 63);
    }

    /* Allocate working bitsets. */
    uint64_t *D  = (uint64_t*)calloc(W, sizeof(uint64_t));
    uint64_t *x  = (uint64_t*)calloc(W, sizeof(uint64_t));
    uint64_t *y  = (uint64_t*)calloc(W, sizeof(uint64_t));
    uint64_t *z  = (uint64_t*)calloc(W, sizeof(uint64_t));
    if (!D || !x || !y || !z) return (seq_t)0;

    /* Main Myers loop: iterate over the complement string. */
    for (idx_t j = 0; j < n; ++j) {
        unsigned tbit = __builtin_parityll((unsigned long long)j);  /* Thue‑Morse bit */
        unsigned complement = 1 - tbit;                          /* bit of the complement */
        const uint64_t *cur = (complement == 0) ? M0 : M1;        /* bits of A matching current B[j] */

        /* x = cur | D */
        bitset_or(cur, D, x, W);

        /* y = (D << 1) | 1 */
        bitset_shift_left(D, y, W);
        y[0] |= 1ULL;

        /* z = x - y */
        bitset_subtract(x, y, z, W);

        /* D = x & ~z */
        for (size_t i = 0; i < W; ++i)
            D[i] = x[i] & ~z[i];
    }

    /* Count bits set in D: the LCS length. */
    size_t lcs = 0;
    for (size_t i = 0; i < W; ++i)
        lcs += __builtin_popcountll(D[i]);

    /* Free allocated memory. */
    free(M0);
    free(M1);
    free(D);
    free(x);
    free(y);
    free(z);

    return (seq_t)lcs;
}
