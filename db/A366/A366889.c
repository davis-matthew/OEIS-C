/*
 * A366889  Dirichlet inverse of the highest power of two that divides the sum
 *          of divisors of n.
 * https://oeis.org/A366889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 5102, i32 n <= 9388, i64 n <= 9388,
 *            i128 n <= 9388, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, for n>1,
   a(n) = -∑_{d|n, d<n} A082903(n/d) * a(d)
   The function uses the multiplicativity of A082903
   and computes the Dirichlet inverse on prime powers. */

#include <stdint.h>
#include <stdbool.h>

/* small helper: count trailing zeros of a 64‑bit integer */
static inline unsigned v2_uint64(uint64_t x)
{
    unsigned c = 0;
    while ((x & 1ULL) == 0ULL)
    {
        ++c;
        x >>= 1;
    }
    return c;
}

/* Dirichlet inverse of A082903 for a single prime power p^e.
   Returns g(p^e), where g = a on that prime power. */
static seq_t g_prime_power(uint64_t p, unsigned e)
{
    /* array large enough for any exponent (max 63 for 64‑bit n) */
    seq_t g[65];
    g[0] = 1;

    const unsigned vp1 = (p == 2) ? 0 : v2_uint64(p + 1);

    for (unsigned k = 1; k <= e; ++k)
    {
        __int128 sum = 0; /* accumulate in 128‑bit to avoid overflow */

        for (unsigned j = 1; j <= k; ++j)
        {
            seq_t f;
            if (p == 2)
            {
                f = 1;
            }
            else
            {
                /* f_j = A082903(p^j) */
                if ((j & 1) == 0)          /* j even → j+1 odd → sigma odd */
                {
                    f = 1;
                }
                else                        /* j odd → j+1 even */
                {
                    unsigned exp = vp1 + v2_uint64(j + 1) - 1;
                    f = ((seq_t)1) << exp;
                }
            }
            sum += (__int128)f * (__int128)g[k - j];
        }
        g[k] = (seq_t)(-sum);
    }
    return g[e];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            seq_t g = g_prime_power(p, e);
            result = (seq_t)((__int128)result * (__int128)g);
            if (result == 0)   /* early exit if zero product */
                return 0;
        }
    }
    if (m > 1) /* remaining prime factor */
    {
        seq_t g = g_prime_power(m, 1);
        result = (seq_t)((__int128)result * (__int128)g);
    }
    return result;
}
