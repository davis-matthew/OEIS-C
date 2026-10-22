/*
 * A341103  T(n, k) = Sum_{j=0..k}(binomial(n + k - j, 2*k) - binomial(n + j -
 *          1, 2*k)) for n > 0 and T(0, 0) = 1. Triangle read by rows, T(n, k)
 *          for 0 <= k <= n.
 * https://oeis.org/A341103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑major flattening of the triangle T(n,k) =
 *            binomial(n + k, 2*k) – binomial(n - 1, 2*k) for n>0, with T(0,0)
 *            = 1. The index n is 0‑based
 * Range:     u8 n <= 22, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the helper binomial(idx_t, idx_t) supplied by the build. */

#include <stdbool.h>
#include "../utils/binomial.h"

static inline idx_t triangular(idx_t i) {
    /* i*(i+1)/2, computed in 128 bit to avoid overflow. */
    __uint128_t t = (__uint128_t)i * (i + 1) / 2;
    return (idx_t)t;
}

/* Return the largest i such that i*(i+1)/2 <= n.  */
static idx_t row_of(idx_t n) {
    idx_t low = 0, high = n + 1;          /* high is exclusive bound */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t i = row_of(n);                 /* row number */
    idx_t j = n - triangular(i);         /* column index in that row */

    if (i == 0)                          /* only entry is T(0,0) = 1 */
        return (seq_t)1;

    seq_t t1 = binomial(i + j, 2 * j);    /* binomial(i+j, 2*j)      */
    seq_t t2 = binomial(i - 1, 2 * j);    /* binomial(i-1, 2*j)      */
    return t1 - t2;
}
