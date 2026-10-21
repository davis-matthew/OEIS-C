/*
 * A259363  Number of distinct elements in the Gram matrix of the first M rows
 *          of the Kronecker product (Sylvester) Hadamard matrix.
 * https://oeis.org/A259363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct values of * S(d) = Σ_{r=0}^{n-1}
 *            (-1)^{popcount(r & d)} * for d = 0 … 2^⌈log₂n⌉-1 * (the values
 *            of the Walsh‑Hadamard transform of the indicator * of {0,…,n-1})
 * Range:     u8 n <= 138, u16 n <= 138, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* compute 2^k >= n */
    size_t N = 1;
    while (N < n) N <<= 1;

    /* indicator vector f[0..N-1] */
    int *f = (int *)malloc(N * sizeof(int));
    if (!f) return (seq_t)0;
    for (size_t i = 0; i < N; ++i) f[i] = (i < n) ? 1 : 0;

    /* Fast Walsh–Hadamard Transform */
    for (size_t len = 1; len < N; len <<= 1) {
        for (size_t i = 0; i < N; i += 2 * len) {
            for (size_t j = 0; j < len; ++j) {
                int u = f[i + j];
                int v = f[i + j + len];
                f[i + j]     = u + v;
                f[i + j + len] = u - v;
            }
        }
    }

    /* count distinct values, using shift by n to make indices non‑negative */
    size_t seen_size = 2 * n + 1;
    unsigned char *seen = (unsigned char *)calloc(seen_size, 1);
    if (!seen) {
        free(f);
        return (seq_t)0;
    }
    for (size_t i = 0; i < N; ++i) {
        int val = f[i];
        size_t idx = (size_t)( (int)n + val );
        seen[idx] = 1;
    }

    size_t count = 0;
    for (size_t i = 0; i < seen_size; ++i) {
        if (seen[i]) ++count;
    }

    free(f);
    free(seen);
    return (seq_t)count;
}
