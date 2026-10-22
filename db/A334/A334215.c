/*
 * A334215  T(n, k) is the greatest positive integer m such that m^k divides
 *          n; square array T(n, k), n, k > 0 read by antidiagonals downwards.
 * https://oeis.org/A334215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(i , j) where (i , j) are the coordinates of the
 * n‑th term in the square array T(n, k) read by antidiagonals downwards.
 * T(n , k) is the greatest m such that m^k divides n; equivalently,
 *
 *   T(n , k) = ∏_{p^e‖n} p^{⌊e/k⌋}
 *
 * (⊳ p^e‖n means that e is the exponent of prime p in n).
 */

#include <stdint.h>

/* helper: binary exponentiation for seq_t */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t r = 1;
    while (exp)
    {
        if (exp & 1) r *= base;
        base *= base;
        exp >>= 1;
    }
    return r;
}

/* greatest m such that m^k divides n */
static seq_t T_val(idx_t n, idx_t k)
{
    if (n == 0) return 1;            /* safety, though n≥1 in use */
    seq_t result = 1;
    uint64_t nn = (uint64_t)n;
    uint64_t cnt = 0;

    /* factor 2 explicitly */
    while ((nn & 1ULL) == 0)
    {
        nn >>= 1;
        ++cnt;
    }
    if (cnt)
    {
        uint64_t exp = cnt / k;
        if (exp) result *= pow_seq((seq_t)2, exp);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= nn; p += 2)
    {
        if (nn % p == 0)
        {
            cnt = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++cnt;
            }
            uint64_t exp = cnt / k;
            if (exp) result *= pow_seq((seq_t)p, exp);
        }
    }

    /* remaining prime factor > 1 */
    if (nn > 1)
    {
        uint64_t exp = 1 / k;  /* 1 only if k==1 */
        if (exp) result *= pow_seq((seq_t)nn, exp);
    }

    return result;
}

/* map n to (i, j) on the antidiagonal and evaluate T */
seq_t a(idx_t n)
{
    /* find the diagonal d such that d*(d+1)/2 ≥ n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;

    idx_t prev = d * (d - 1) / 2;          /* terms before diagonal d */
    idx_t offset = n - prev - 1;          /* 0‑based position within the diagonal */
    idx_t i = offset + 1;                 /* row index (n) */
    idx_t j = d - offset;                 /* column index (k) */

    return T_val(i, j);
}
