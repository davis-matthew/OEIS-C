/*
 * A368467  a(n) = Sum_{k=0..n} (-1)^k * binomial(2*n,k) * binomial(4*n,n-k).
 * https://oeis.org/A368467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(2*n,k) * binomial(4*n,n-k)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A368467.  For seq_t that is a signed integer
 * of any width or an arbitrary–precision integer type, this routine uses
 * only 128‑bit intermediate arithmetic (guaranteed to hold all values
 * that fit in seq_t when n is such that a(n) < 2^63).  The sum is stored
 * in a 128‑bit variable and then cast back to seq_t before returning. */

#include <stdint.h>

/* ---------- 128‑bit binomial coefficient for idx_t ---------------- */
static inline __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry */
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i);        /* multiply by numerator */
        res = res / i;                  /* exact division by i   */
    }
    return res;
}

/* ===================== Sequence function ========================= */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* a(0) = 1 */

    __int128 sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        __int128 c1 = binom128(2 * n, k);
        __int128 c2 = binom128(4 * n, n - k);
        __int128 term = c1 * c2;

        if (k & 1)
            sum -= term;
        else
            sum += term;
    }

    return (seq_t)sum;     /* cast back to seq_t (works for all supported types) */
}
