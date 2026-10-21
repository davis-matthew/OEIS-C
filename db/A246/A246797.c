/*
 * A246797  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation Sum_{k=0..n} (k+1)*x^k = Sum_{k=0..n} A_k*(x-2)^k.
 * https://oeis.org/A246797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

/*  Compute a(n) for OEIS sequence A246797.
 *
 *  a(n) = T(r,k) where
 *      r = floor( (sqrt(1+8n) - 1) / 2 )
 *      k = n - r(r+1)/2
 *  T(r,k) = Σ_{m=k..r} (m+1) * C(m,k) * 2^(m-k)
 *
 *  The function works for any seq_t and idx_t defined by the build.
 *  It uses only +, -, *, /, %, <<, >>, &, |, ^ and comparisons on seq_t,
 *  and only standard library functions.
 */

static inline idx_t isqrt_u128(unsigned __int128 x) {
    long double d = sqrtl((long double)x);
    idx_t r = (idx_t)d;
    /* Correct possible rounding errors */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) ++r;
    while ((unsigned __int128)r * r > x) --r;
    return r;
}

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    unsigned __int128 t = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t root = isqrt_u128(t);
    idx_t r = (root - 1) / 2;

    /* Column k within the row */
    idx_t prefix = r * (r + 1) / 2;
    idx_t k = n - prefix;

    /* Compute T(r,k) = Σ_{m=k}^{r}  (m+1) * C(m,k) * 2^(m-k) */
    seq_t sum = 0;
    for (idx_t m = k; m <= r; ++m) {
        seq_t term = binomial(m, k);                  /* C(m,k)   */
        term = term * (seq_t)(m + 1);                 /* (m+1)    */
        term = term * ((seq_t)1 << (unsigned)(m - k));/* 2^(m-k)  */
        sum += term;
    }
    return sum;
}
