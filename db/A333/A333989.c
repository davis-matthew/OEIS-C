/*
 * A333989  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of (1+(k-1)*x) /
 *          (1+2*(k-1)*x+((k+1)*x)^2).
 * https://oeis.org/A333989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the entry T(i,j) of an infinite square array read * by
 *            antidiagonals. T(n,k) = Σ_{j=0..n} (-k)^j * binomial(2n,2j). *
 *            Equivalent recurrence: T(0,k)=1; T(1,k)=1-k; * For n>1, T(n,k) =
 *            -2*(k-1)*T(n-1,k) - (k+1)^2*T(n-2,k)
 * Range:     i8 n <= 32, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square‑root function for unsigned __int128.  Returns floor(sqrt(x)). */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 34;          /* > sqrt(8*2^64+1) */
    while (lo < hi)
    {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute T(n,k) by the recurrence.  n,k are non‑negative indices. */
static seq_t T_entry(idx_t n, idx_t k)
{
    if (n == 0)
        return 1;

    if (n == 1)
        return 1 - (seq_t)k;

    seq_t t0 = 1;                     /* T(0,k) */
    seq_t t1 = 1 - (seq_t)k;          /* T(1,k) */
    seq_t km1 = (seq_t)k - 1;
    seq_t kpp1 = (seq_t)k + 1;
    seq_t kpp1_sq = kpp1 * kpp1;      /* (k+1)^2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t t2 = -2 * km1 * t1 - kpp1_sq * t0;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}

/* Main sequence entry.  Maps linear index n to the diagonal
 * containing it, then finds row i and column j and returns T(i,j). */
seq_t a(idx_t n)
{
    /* Diagonal m containing the term:  m = floor((sqrt(8*n+1)-1)/2). */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t s   = isqrt_uint128(val);
    idx_t m   = (s - 1) / 2;

    /* Number of terms before diagonal m: T(m,0)+…+T(m,m-1). */
    idx_t prev = m * (m + 1) / 2;

    /* Row i and column j within this diagonal. */
    idx_t i = n - prev;
    idx_t j = m - i;

    return T_entry(i, j);
}
