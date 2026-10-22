/*
 * A334491  a(n) = Product_{d|n} gcd(d, sigma(d)).
 * https://oeis.org/A334491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} gcd(d, sigma(d))
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 239, u64 n <= 839,
 *            u128 n <= 2519, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helpers from utils */
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

/* Returns the prime factorisation of n.
   Prime factors and exponents are stored in the caller–provided
   arrays; the number of distinct primes is written to *cnt. */
static void factor(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1)            /* remaining prime factor */
    {
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively enumerate all divisors of n using its factorisation.
   For each divisor d the product is multiplied by gcd(d, sigma(d)). */
static void dfs_div(size_t idx, uint64_t cur,
                    const uint64_t *pr, const uint64_t *ex,
                    size_t num, seq_t *res)
{
    if (idx == num)
    {
        seq_t d   = (seq_t)cur;
        seq_t sig = sigma_of((idx_t)cur);
        *res = (*res) * gcd(d, sig);
        return;
    }

    uint64_t p = pr[idx];
    uint64_t e = ex[idx];
    uint64_t mult = 1;

    for (uint64_t k = 0; k <= e; ++k)
    {
        dfs_div(idx + 1, cur * mult, pr, ex, num, res);
        mult *= p;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    /* Sufficient room for prime factors of numbers up to ~10^12 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t num = 0;

    factor((uint64_t)n, primes, exps, &num);

    seq_t result = 1;
    dfs_div(0, 1, primes, exps, num, &result);
    return result;
}
