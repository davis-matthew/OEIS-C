/*
 * A345311  a(n) = Sum_{p|n, p prime} p^lcm(p,n/p).
 * https://oeis.org/A345311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^lcm(p,n/p)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow, gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t */
#include "../utils/prime_pi.h"   /* for _smallest_prime_factor if needed */
#include "../utils/big_pow.h"    /* big_pow(seq_t base, seq_t exp) */
#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b) */

static /*inline*/ uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;
    /* working copy for factorisation */
    uint64_t nn = (uint64_t)n;
    const uint64_t orig_n = nn;          /* keep original n */

    /* handle factor 2 explicitly */
    if ((nn & 1ULL) == 0ULL)
    {
        uint64_t p = 2ULL;
        uint64_t n_div_p = orig_n / p;
        seq_t g = gcd((seq_t)p, (seq_t)n_div_p);
        seq_t l = ((seq_t)p / g) * (seq_t)n_div_p;
        result += big_pow((seq_t)p, l);
        while ((nn & 1ULL) == 0ULL)
            nn >>= 1;                    /* divide by 2 until odd */
    }

    /* loop over odd candidates up to sqrt(nn) */
    uint64_t limit;
    for (uint64_t f = 3ULL; f * f <= nn; f += 2ULL)
    {
        if (nn % f == 0ULL)
        {
            uint64_t p = f;
            uint64_t n_div_p = orig_n / p;
            seq_t g = gcd((seq_t)p, (seq_t)n_div_p);
            seq_t l = ((seq_t)p / g) * (seq_t)n_div_p;
            result += big_pow((seq_t)p, l);
            while (nn % p == 0ULL)
                nn /= p;                  /* strip all copies of p */
        }
    }

    /* if a prime factor larger than sqrt remains */
    if (nn > 1ULL)
    {
        uint64_t p = nn;                   /* nn is now prime */
        uint64_t n_div_p = orig_n / p;
        seq_t g = gcd((seq_t)p, (seq_t)n_div_p);
        seq_t l = ((seq_t)p / g) * (seq_t)n_div_p;
        result += big_pow((seq_t)p, l);
    }

    return result;
}
