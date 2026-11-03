/*
 * A345935  Number of divisors d of n for which A002034(d) = A002034(n), where
 *          A002034(n) is the smallest positive integer k such that n divides
 *          k!.
 * https://oeis.org/A345935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n with A002034(d) = A002034(n)
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9840 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                A002034(n) is the smallest k such that n | k!          */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
static void factorise_u64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) { *cnt = 0; return; }
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
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1)
    {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* ------------------------------------------------------------ */
static seq_t min_k_prime_power(uint64_t p, int e)
{
    /* The value of k is guaranteed to be <= n for the problem size.
       We use the 64‑bit integer type for intermediate arithmetic.  */
    idx_t k = 0;
    while (1)
    {
        ++k;
        idx_t t = p;
        idx_t sum = 0;
        while (t <= k)
        {
            sum += k / t;
            if (t > k / p) break; /* avoid overflow of t *= p   */
            t *= p;
        }
        if (sum >= e) break;
    }
    return (seq_t)k;
}

/* ------------------------------------------------------------ */
static seq_t smarandache_u64(uint64_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factorise_u64(n, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t k = min_k_prime_power(primes[i], exps[i]);
        if (k > res) res = k;
    }
    return res;
}

/* ------------------------------------------------------------ */
/* Factorisation of a 64‑bit number (trial division).          */
/* primes  : array where prime factors are stored                 */
/* exps    : array where corresponding exponents are stored       */
/* *cnt    : number of distinct prime factors                     */

/* ------------------------------------------------------------ */
/* For a prime power p^e, return the minimal k such that p^e | k! */

/* ------------------------------------------------------------ */
/* Kempner (Smarandache) function for a 64‑bit integer.        */

/* ------------------------------------------------------------ */
/* Recursively enumerate all divisors of n using its factorisation */
/* ------------------------------------------------------------ */
static void rec_divisors(size_t idx, size_t total,
                         const uint64_t *pr, const int *ex,
                         int *curExp,
                         seq_t Smn,
                         seq_t *cnt)
{
    if (idx == total)
    {
        /* Compute smarandache value for this divisor. */
        seq_t Sd = 0;
        bool hasPrime = false;
        for (size_t j = 0; j < total; ++j)
        {
            if (curExp[j] > 0)
            {
                hasPrime = true;
                seq_t t = min_k_prime_power(pr[j], curExp[j]);
                if (t > Sd) Sd = t;
            }
        }
        if (!hasPrime) Sd = 1;          /* divisor = 1 */
        if (Sd == Smn) (*cnt) += (seq_t)1;
        return;
    }

    for (int e = 0; e <= ex[idx]; ++e)
    {
        curExp[idx] = e;
        rec_divisors(idx + 1, total, pr, ex, curExp, Smn, cnt);
    }
}

/* ------------------------------------------------------------ */
/* Main entry point for sequence A345935.                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t Smn = smarandache_u64(N);

    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factorise_u64(N, primes, exps, &cnt);

    int curExp[64] = {0};
    seq_t result = (seq_t)0;

    rec_divisors(0, cnt, primes, exps, curExp, Smn, &result);
    return result;
}
