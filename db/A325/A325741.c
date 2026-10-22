/*
 * A325741  First term of n-th difference sequence of (floor(Pi*k/2)), k >= 0.
 * https://oeis.org/A325741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = first term of the n‑th forward difference of * floor(pi *
 *            k / 2), k >= 0. * * The function is fully integer‑only, uses a
 *            15‑digit rational * approximation of pi that is exact for all k
 *            <= 10000, * and runs in O(n²) time (≤ 5×10⁷ elementary
 *            operations for * n = 10000). It requires only <std...
 * Range:     i8 n <= 9, i16 n <= 18, i32 n <= 34, i64 n <= 67, i128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned long long NUM = 1570796326794897ULL;   /* round(pi*1e15/2) */
static const unsigned long long DEN = 1000000000000000ULL;   /* 1e15 */

seq_t a(idx_t n)
{
    /* Allocate array to hold f[0..n] */
    seq_t *f = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!f) return 0;                     /* unlikely – out of memory */

    /* Compute f(k) = floor(pi * k / 2) using an exact rational */
    for (idx_t k = 0; k <= n; ++k) {
        /* use unsigned __int128 to avoid overflow of k*NUM */
        unsigned __int128 prod = (unsigned __int128)k * NUM;
        f[k] = (seq_t)(prod / DEN);
    }

    /* Successively apply n forward differences in place */
    idx_t len = n + 1;
    for (idx_t step = 0; step < n; ++step) {
        for (idx_t i = 0; i + 1 < len; ++i) {
            f[i] = f[i + 1] - f[i];
        }
        --len;
    }

    seq_t result = f[0];
    free(f);
    return result;
}
