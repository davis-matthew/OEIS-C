/*
 * A242598  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation of 1 + x + x^2 + ... + x^n to the polynomial
 *          A_k*(x-k)^k for 0 <= k <= n.
 * https://oeis.org/A242598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient T(r,k) where * 1 + x + … + x^r =
 *            Σ_{k=0}^r T(r,k)*(x-k)^k * The triangle is indexed by rows r =
 *            0,1,… and columns k = 0…r. * The flattened sequence a(n) is read
 *            by rows: a(0)=T(0,0), * a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), …
 *            * We locate (r,k) from the index n and then com...
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility: integer square‑root of a 128‑bit value. */
static inline idx_t isqrt_u128(__int128 v) {
    idx_t lo = 0, hi = (idx_t)-1;          /* hi fits 64 bits */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 m2 = (__int128)mid * mid;
        if (m2 <= v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Small power for seq_t: base^exp, exp >= 0. */
static inline seq_t seq_pow_small(seq_t base, unsigned exp) {
    seq_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* Include the provided binomial function. */
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r;
    {
        __int128 tmp = (__int128)8 * n + 1;
        idx_t s = isqrt_u128(tmp);
        r = (s - 1) / 2;
    }

    /* Column index within the row. */
    idx_t kcol = n - r * (r + 1) / 2;
    if (r == 0)              /* only T(0,0)=1 */
        return (seq_t)1;

    /* Allocate array for the r+1 coefficients. */
    seq_t *T = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!T)
        return (seq_t)0;      /* allocation failure (unlikely) */

    T[r] = 1;                /* base case */

    /* Backward recurrence for j = r-1 … 0 */
    for (idx_t j = r; j-- > 0; ) {
        seq_t sum_even = 0;
        seq_t sum_odd  = 0;
        for (idx_t m = j + 1; m <= r; ++m) {
            seq_t bin = binom(m, j);
            seq_t powk = seq_pow_small(m, (unsigned)(m - j));
            seq_t term = T[m];
            term *= bin;
            term *= powk;
            if ((m - j) & 1u)
                sum_odd += term;
            else
                sum_even += term;
        }
        /* T[j] = 1 - (sum_even - sum_odd) = 1 + sum_odd - sum_even */
        T[j] = sum_odd + 1 - sum_even;
    }

    seq_t result = T[kcol];
    free(T);
    return result;
}
