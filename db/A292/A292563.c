/*
 * A292563  Expansion of Product_{k>=1} (1 + x^((2*k-1)^3)) / (1 -
 *          x^((2*k-1)^3)).
 * https://oeis.org/A292563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 431, u16 n <= 2403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient of x^n in
 *     Π_{k≥1} (1 + x^{(2k-1)^3}) / (1 - x^{(2k-1)^3}).
 *
 * This is computed by dynamic programming.  For each odd cube
 * t = (2k-1)^3 the factor (1 + x^t)/(1 - x^t) expands to
 *     1 + 2x^t + 2x^{2t} + 2x^{3t} + … .
 * Consequently if f[n] is the current coefficient sequence,
 * after processing this t we have
 *     new_f[n] = f[n] + 2*∑_{j≥1} f[n - j*t].
 * The sum over equal residues modulo t can be accumulated
 * while iterating, giving an O(n) update for each t.
 */

#ifdef __cplusplus
# include <cstddef>
# include <cstdint>
#else
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
#endif

/* Helper allocation that works in both C and C++ */
static inline seq_t* alloc_seq(idx_t len) {
#ifdef __cplusplus
    return new seq_t[len];
#else
    return (seq_t*)calloc((size_t)len, sizeof(seq_t));
#endif
}
static inline void free_seq(seq_t *p) {
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* Allocate coefficient array up to n, initialise to 0. */
    seq_t *f = alloc_seq(n + 1);
    f[0] = 1;                                   /* a(0) = 1 */

    /* Process odd cubes t = (2*k-1)^3 */
    for (idx_t k = 1; ; ++k) {
        idx_t m = 2*k - 1;
        idx_t t = m * m * m;                    /* t <= n ? */
        if (t > n) break;

        seq_t *newf = alloc_seq(n + 1);

        /* For each residue modulo t compute cumulative sums. */
        for (idx_t r = 0; r < t; ++r) {
            seq_t sum = 0;
            for (idx_t i = r; i <= n; i += t) {
                /* newf[i] = f[i] + 2*sum; */
                seq_t twice = sum << 1;          /* 2*sum */
                newf[i] = f[i] + twice;
                sum += f[i];
            }
        }

        free_seq(f);
        f = newf;
    }

    seq_t result = f[n];
    free_seq(f);
    return result;
}
