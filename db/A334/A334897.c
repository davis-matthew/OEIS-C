/*
 * A334897  a(n) is the number of positive integers less than or equal to n
 *          that can be written as the geometric mean of two different
 *          positive integers less than or equal to n.
 * https://oeis.org/A334897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the positive integers m≤n such that m is the
 *            geometric mean of two distinct positive integers ≤n. Algorithm:
 *            for each m (2≤m≤n) factor m, enumerate all divisors d of m² with
 *            d<m, and check if the paired divisor b=m²/d satisfies b≤n.
 *            Because d<m≤n, only the bound on b remains to be tes...
 * Range:     u8 n <= 31, u16 n <= 519, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper to test whether a given m is the geometric mean of two
   distinct integers ≤n. */
static bool _gm_candidate(idx_t m, idx_t n)
{
    if (m <= 1)                 /* 1 cannot be a GM of two distinct numbers */
        return false;

    /* Compute ceil(m^2 / n) – requires only idx_t arithmetic. */
    idx_t low;
    {
        /* careful about overflow: m^2 fits in uint64_t for m≤10^4. */
        uint64_t m2 = (uint64_t)m * (uint64_t)m;
        low = (idx_t)((m2 + (uint64_t)n - 1ULL) / (uint64_t)n);
    }

    if ((idx_t)m <= low)        /* no divisor d can satisfy d< m and d≥low */
        return false;

    /* Factor m.  At most 6 distinct primes for m≤10⁴, tiny arrays suffice. */
    idx_t primes[6];
    idx_t exps[6];
    size_t pcnt = 0;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            size_t e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1)                /* remaining prime factor */
    {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Enumerate all divisors of m².  The largest possible number of
       divisors here is (8+1)*(8+1)=81 for 10000²; 4096 is ample. */
    seq_t divs[4096];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i)
    {
        size_t old = dcnt;
        seq_t mult = 1;
        for (size_t e = 1; e <= 2 * exps[i]; ++e)
        {
            mult *= (seq_t)primes[i];
            for (size_t j = 0; j < old; ++j)
            {
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* Check the divisors that are < m.  as soon as we find one which
       pairs to b≤n the candidate is valid. */
    seq_t m2 = (seq_t)m * (seq_t)m;
    for (size_t i = 0; i < dcnt; ++i)
    {
        seq_t d = divs[i];
        if (d < (seq_t)m && d >= (seq_t)low)
        {
            seq_t b = m2 / d;
            if (b <= (seq_t)n)
                return true;
        }
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t m = 2; m <= n; ++m)
        if (_gm_candidate(m, n))
            ++cnt;
    return cnt;
}
