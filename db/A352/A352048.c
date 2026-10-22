/*
 * A352048  Sum of the squares of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * Sum_{d|n, d<n, d odd} 1/d^2
 * Range:     u8 n <= 14, u16 n <= 239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form: a(n) = Sum_{odd d|n, d<n} (n/d)^2 */

#include <stdint.h>
#include <stddef.h>

/* Local helper to factor a number into primes and exponents.
   Since n <= 10⁴ in the test range, trial division is fast enough. */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    if (n == 0) return;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[(*cnt)++] = p;
            exps[(*cnt) - 1] = e;
            if (*cnt >= 16) break; /* safe guard */
        }
    }
    if (n > 1)
    {
        primes[(*cnt)++] = n;
        exps[(*cnt) - 1] = 1;
    }
}

/* Local helper to generate all divisors from prime factorization.
   divs must be large enough (here 256 works for n <= 10⁴). */
static size_t generate_divisors(const uint64_t *primes, const int *exps,
                                size_t cnt_factors, uint64_t *divs,
                                size_t max_divs)
{
    size_t div_cnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt_factors; ++i)
    {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t old_cnt = div_cnt;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k)
        {
            p_pow *= p;
            for (size_t j = 0; j < old_cnt; ++j)
            {
                if (div_cnt + 1 < max_divs) /* ensure no overflow */
                    divs[div_cnt++] = divs[j] * p_pow;
            }
        }
    }
    return div_cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Remove all powers of two to obtain the odd part of n */
    uint64_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;

    /* Factor the odd part */
    uint64_t primes[16];
    int exp[16];
    size_t pf_cnt = 0;
    factor_uint64(m, primes, exp, &pf_cnt);

    /* Generate all odd divisors of n (i.e., divisors of the odd part) */
    uint64_t divs[256];
    size_t div_cnt = generate_divisors(primes, exp, pf_cnt, divs, 256);

    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d = divs[i];
        if (d == (uint64_t)n) continue; /* skip n itself (odd case) */
        seq_t q = (seq_t)n / (seq_t)d;    /* integer division, q = n/d */
        sum += q * q;
    }

    return sum;
}
