/*
 * A286880  Square array A(n,k), n>=0, k>=1, read by antidiagonals, where row
 *          n is the sum of n-th powers of unitary divisors of k (divisors d
 *          such that gcd(d, k/d) = 1).
 * https://oeis.org/A286880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A286880
   A(n,k) = Σ_{d|k, gcd(d,k/d)=1} d^n  =  Π_{p^e‖k} (1 + (p^e)^n)
   Sequence a(n) is obtained by reading the table by antidiagonals.
   a(0) = 1, a(1) = 2, a(2) = 1, a(3) = 2, … (the values listed in the
   OEIS entry) */

#include <stddef.h>
#include <stdint.h>
#include <math.h>   /* for sqrt (used only on small idx_t values) */

/* fast exponentiation for seq_t base, idx_t exponent */
static inline seq_t pow_uint_local(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1ULL)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* integer square root of an idx_t (floor) – uses double for speed
   (values are small for the required n range, so no precision loss) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = (idx_t)sqrt((double)n);
    while ((r + 1) * (r + 1) <= n)
        ++r;
    while (r * r > n)
        --r;
    return r;
}

/* Return the n‑th term of A286880.  External linkage required. */
seq_t a(idx_t n)
{
    /* Compute the diagonal d such that
       d*(d+1)/2 ≤ n < (d+1)*(d+2)/2. */
    idx_t d = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    /* Adjust in case of rounding error. */
    while (d * (d + 1) / 2 > n)
        --d;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Position inside the diagonal (0‑based). */
    idx_t pos  = n - d * (d + 1) / 2;
    idx_t row  = pos;          /* r = row index   */
    idx_t col  = d - pos + 1;  /* k = column   (1‑based) */

    /* Factorise col into prime powers.  col is small (≤ d+1). */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    uint64_t v = col;
    if (v > 1)
    {
        if ((v & 1ULL) == 0)
        {
            uint64_t e = 0;
            while ((v & 1ULL) == 0)
            {
                ++e;
                v >>= 1;
            }
            primes[cnt] = 2ULL;
            exps[cnt]   = e;
            ++cnt;
        }

        uint64_t limit = isqrt_u64(v);
        for (uint64_t p = 3; p <= limit; p += 2)
        {
            if (v % p == 0)
            {
                uint64_t e = 0;
                while (v % p == 0)
                {
                    ++e;
                    v /= p;
                }
                primes[cnt] = p;
                exps[cnt]   = e;
                ++cnt;
                limit = isqrt_u64(v);
            }
        }

        if (v > 1)
        {
            primes[cnt] = v;
            exps[cnt]   = 1;
            ++cnt;
        }
    }

    /* Compute A(row,col) = product_{p^e||col} (1 + (p^e)^row) */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p_pow_e = pow_uint_local((seq_t)primes[i], (idx_t)exps[i]); /* p^e          */
        seq_t term    = 1 + pow_uint_local(p_pow_e, row);                /* 1 + (p^e)^row */
        result *= term;
    }

    return result;
}
