/*
 * A392337  Triangle read by rows: T(n,k) = Sum_{j=0..2k} (-1)^j *
 *          binomial(2k,j) * (1+k-j)^(2n).
 * https://oeis.org/A392337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2, and
 *            T(r,k) = Σ_{j=0}^{2k} (-1)^j * binom(2k,j) * (1+k-j)^(2r)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for unsigned indices. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Binomial coefficient C(n,k) for small k; safe for 64‑bit seq_t. */
static inline seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* Integer power: base^exp, exp is even and small (≤ 512). */
static inline seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Determine row (r) and column (k) in the triangular array. */
    idx_t sq  = isqrt_idx(8 * n + 1);
    idx_t r   = (sq - 1) / 2;
    idx_t prev = r * (r + 1) / 2;
    idx_t k   = n - prev;
    if (k == 0) return 1;          /* T(r,0) = 1 */

    seq_t pos = 0, neg = 0;
    idx_t two_r = 2 * r;
    for (idx_t j = 0; j <= 2 * k; ++j) {
        seq_t bin = binom_seq(2 * k, j);

        /* The base (1+k-j) may be negative; use its absolute value. */
        int64_t base_int = (int64_t)(k + 1) - (int64_t)j;
        seq_t base_abs   = (base_int >= 0) ? (seq_t)base_int : (seq_t)(-base_int);

        seq_t pow_val = ipow(base_abs, two_r);
        seq_t term    = bin * pow_val;

        if ((j & 1ULL) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;
}
