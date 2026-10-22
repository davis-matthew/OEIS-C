/*
 * A304334  T(n, k) = Sum_{j=0..k} (-1)^j*binomial(2*k, j)*(k - j)^(2*n)/k!,
 *          triangle read by rows, n >= 0 and 0 <= k <= n.
 * https://oeis.org/A304334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2 T(n,k) = 1/k! * Σ_{j=0..k} (-1)^j * C(2k,j) *
 *            (k-j)^(2n) (index n starts at 0, a(0)=1)
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for size_t, but types seq_t and idx_t are supplied by the build */

static inline seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

static inline seq_t
binomial(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);        /* multiply by numerator term */
        res = res / i;                 /* exact division by i */
    }
    return res;
}

static inline seq_t
factorial(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i) f = f * i;
    return f;
}

seq_t a(idx_t n)
{
    /* find row r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* column k in that row */
    idx_t k = n - r * (r + 1) / 2;

    /* factorial(k) for division later */
    seq_t fact_k = factorial(k);

    /* accumulate numerator of T(r,k) */
    seq_t sum = 0;

    /* binomial(2k,0)=1; compute in place */
    seq_t bin = 1;
    for (idx_t j = 0; j <= k; ++j) {
        /* ((-1)^j) handled by sign of j */
        seq_t pow = pow_seq(k - j, 2 * r);      /* (k-j)^(2r) */
        seq_t term = bin * pow;                 /* binomial * power */

        if (j & 1) {           /* subtract for odd j */
            sum = sum - term;   /* exact because term <= sum for the given k */
        } else {                /* add for even j */
            sum = sum + term;
        }

        /* update binomial for next j (avoid recomputing from scratch) */
        if (j < k) {
            /* bin_{j+1} = bin_j * (2k - j) / (j + 1) */
            bin = bin * (2 * k - j) / (j + 1);
        }
    }

    /* division by k! gives the final integer value */
    return sum / fact_k;
}
