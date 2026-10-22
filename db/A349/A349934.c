/*
 * A349934  Array read by ascending antidiagonals: A(n, s) is the n-th
 *          s-Catalan number.
 * https://oeis.org/A349934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 38, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A349934 – array read by ascending antidiagonals of s‑Catalan numbers
 * A(n,s) = T(2n, s*n, s) – T(2n, s*n+1, s)
 * where T(m,k,s) is the coefficient of x^k in (1+x+⋯+x^s)^m.
 * The sequence a(n) enumerates A(n,s) by ascending anti‑diagonals
 * of the infinite array indexed by (n,s) ≥ 1.
 */

#include <stdint.h>
#include "../utils/binom.h"

static idx_t isqrt_u64(idx_t x)
{
    /* floor(sqrt(x)) using 128‑bit multiplication to avoid overflow */
    idx_t lo = 0, hi = 4294967295ULL, ans = 0;          /* 2^32–1 = ⌊√(2^64−1)⌋ */
    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
        {
            ans = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return ans;
}

/* coefficient T(n,k,s) for (1+x+⋯+x^s)^n */
static seq_t t_coef(idx_t n, idx_t k, idx_t s)
{
    if (k > n * s)
        return 0;

    idx_t maxj = k / (s + 1);
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t j = 0; j <= maxj; ++j)
    {
        idx_t term_k = k - j * (s + 1) + n - 1;
        seq_t term = binom(n, j) * binom(term_k, n - 1);
        if (j & 1)
            neg += term;
        else
            pos += term;
    }
    return pos - neg;
}

/* n‑th s‑Catalan number */
static seq_t catalan_n_s(idx_t n, idx_t s)
{
    seq_t t1 = t_coef(2 * n, s * n, s);
    seq_t t2 = t_coef(2 * n, s * n + 1, s);
    return t1 - t2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* zero–based index */
    idx_t k = n - 1;

    /* diagonal index d (1‑based) – smallest d with k < d(d+1)/2 */
    idx_t tmp = 8 * k + 1;
    idx_t d = (isqrt_u64(tmp) - 1) / 2 + 1;

    /* number of terms before this diagonal */
    idx_t cum_prev = (d - 1) * d / 2;

    /* position within the diagonal (0‑based) */
    idx_t t = k - cum_prev;

    /* mapping to (n_val, s_val) */
    idx_t n_val = d - t;   /* row index inside array (≥1) */
    idx_t s_val = t + 1;   /* column index inside array (≥1) */

    return catalan_n_s(n_val, s_val);
}
