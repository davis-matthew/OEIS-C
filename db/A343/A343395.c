/*
 * A343395  a(n) = Sum_{i=1..n} gcd(n^(n-i),n-i).
 * https://oeis.org/A343395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} gcd(n^(n-i), n-i)
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n >= 1.
   Uses a simple prime factorisation of n and exploits that
   gcd(n^k , m) is the product of p^min(vp(m), k·vp(n)) over primes p|n. */
#include <stdint.h>

/* Helper: fast exponentiation in seq_t */
static seq_t pow_seq_uint64(uint64_t base, unsigned int exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Computes a(n). */
seq_t a(idx_t n)
{
    /* Edge case: n == 0 is not defined. */
    if (n == 0)
        return 0;

    /* Factorise n into primes: store primes and exponents. */
    uint64_t primes[16];
    int exps[16];
    int cnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2))
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                e++;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            cnt++;
        }
    }
    if (tmp > 1)
    {
        primes[cnt] = tmp;
        exps[cnt] = 1;
        cnt++;
    }

    seq_t total = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t m = n - i;               /* m = n - i */
        if (m == 0)                    /* gcd(n^0, 0) = 1 */
        {
            total += 1;
            continue;
        }

        seq_t gcd_val = 1;
        for (int j = 0; j < cnt; ++j)
        {
            uint64_t p = primes[j];
            int e_n = exps[j];

            /* exponent of p in m */
            idx_t e_m = 0;
            idx_t temp = m;
            while (temp % p == 0)
            {
                temp /= p;
                e_m++;
            }

            idx_t need = (idx_t)(n - i) * (idx_t)e_n; /* k * vp(n) */
            int exp_in_gcd = (e_m < need) ? (int)e_m : (int)need;

            if (exp_in_gcd > 0)
                gcd_val *= pow_seq_uint64(p, (unsigned int)exp_in_gcd);
        }

        total += gcd_val;
    }

    return total;
}
