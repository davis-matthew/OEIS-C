/*
 * A325739  First term of n-th difference sequence of (floor(Pi*k)), k >= 0.
 * https://oeis.org/A325739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = first term of the n‑th difference sequence of floor(pi*k)
 *            , k >= 0
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 37, i64 n <= 68, i128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(1) = 3, a(2) = 0, … as defined for OEIS A325739. */
#include <stdlib.h>
#include <stdint.h>

static const unsigned long long SCALE  = 1000000000000000000ULL;          /* 10^18  */
static const unsigned long long F_INT  = 141592653589793238ULL;           /* fractional part of pi * SCALE */
static const unsigned long long N      = 3141592653589793238ULL;         /* pi * SCALE (rounded) */

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    /* Allocate array for f(k) = floor(pi*k), k = 0 .. n. */
    size_t len = (size_t)(n + 1);                /* n is guaranteed to fit in size_t for all tests */
    seq_t *f = (seq_t *)malloc(len * sizeof(seq_t));
    if (!f) return (seq_t)0;                      /* safe fallback on allocation failure */

    /* Compute floor(pi*k) using integer arithmetic with a 10^18 scaling. */
    for (size_t k = 0; k < len; ++k) {
        __int128 prod = (__int128)k * N;           /* k * pi*SCALE */
        __int128 div  = prod / SCALE;              /* integer division gives floor(pi*k) */
        f[k] = (seq_t)div;
    }

    /* Perform successive forward differences in place.
       After order i differences, f[0] holds the first term of that difference sequence. */
    for (size_t order = 1; order <= n; ++order) {
        size_t limit = len - order;
        for (size_t i = 0; i < limit; ++i) {
            f[i] = f[i + 1] - f[i];
        }
    }

    seq_t result = f[0];
    free(f);
    return result;
}
