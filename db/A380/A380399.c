/*
 * A380399  The number of nonunitary divisors of n that are perfect powers
 *          (A001597).
 * https://oeis.org/A380399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of nonunitary divisors of n that are perfect
 *            powers
 * Range:     u8 n <= 44, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function follows the definition: a(n) counts d | n, d>1, gcd(d,n/d)>1,
   and d is a perfect power. It works for both integer and arbitrary‑precision
   seq_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Small exponentiation on seq_t (sequential multiply). */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = (seq_t)1;
    while (exp--) result *= base;
    return result;
}

/* Integer n‑th root: largest integer r with r^n <= x. */
static seq_t int_nth_root(seq_t x, unsigned n)
{
    seq_t low  = (seq_t)1;
    seq_t high = x;
    seq_t ans  = (seq_t)1;

    while (low <= high)
    {
        seq_t mid = (low + high) / (seq_t)2;
        seq_t mid_pow = pow_seq(mid, n);

        if (mid_pow <= x)
        {
            ans = mid;
            low = mid + (seq_t)1;
        }
        else
        {
            if (mid == (seq_t)0) break;  /* avoid underflow */
            high = mid - (seq_t)1;
        }
    }
    return ans;
}

/* Checks whether x (>1) is a perfect power. */
static bool is_perfect_power(seq_t x)
{
    if (x <= (seq_t)1) return false;

    /* For n up to 2^64, exponent e never exceeds 64. */
    for (unsigned e = 2; e <= 64; ++e)
    {
        seq_t root = int_nth_root(x, e);
        if (root <= (seq_t)1) continue;
        if (pow_seq(root, e) == x) return true;
    }
    return false;
}

/* Factor n into primes and exponents: primes[i], exps[i] for i<cnt. */
static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i = 0;
    for (uint64_t p = 2; p <= n / p; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            primes[i] = p;
            exps[i]   = e;
            ++i;
        }
    }
    if (n > 1)
    {
        primes[i] = n;
        exps[i]   = 1;
        ++i;
    }
    *cnt = i;
}

/* Recursive enumeration of all divisors. */
static void count_divisors(size_t idx,
                           seq_t cur,
                           const uint64_t *primes,
                           const uint64_t *exps,
                           size_t numPrimes,
                           seq_t n_val,
                           seq_t *cnt)
{
    if (idx == numPrimes)
    {
        if (cur > (seq_t)1)
        {
            seq_t partner = n_val / cur;
            if (gcd(cur, partner) > (seq_t)1)
            {
                if (is_perfect_power(cur)) ++(*cnt);
            }
        }
        return;
    }

    const uint64_t p = primes[idx];
    seq_t cur_pow = (seq_t)1;
    for (uint64_t e = 0; e <= exps[idx]; ++e)
    {
        count_divisors(idx + 1, cur * cur_pow, primes, exps,
                       numPrimes, n_val, cnt);
        cur_pow *= (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t numPrimes = 0;
    factor_uint64((uint64_t)n, primes, exps, &numPrimes);

    seq_t n_val = (seq_t)n;
    seq_t result = (seq_t)0;

    count_divisors(0, (seq_t)1, primes, exps,
                   numPrimes, n_val, &result);

    return result;
}
