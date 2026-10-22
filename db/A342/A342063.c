/*
 * A342063  Primes p such that p^8 - 1 has fewer than 384 divisors.
 * https://oeis.org/A342063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth prime p such that p^8 − 1 has fewer than 384
 *            divisors. * The function builds the sequence by generating all
 *            primes up to a fixed * limit, testing each for the divisor count
 *            of p^8 − 1, and storing the * qualifying primes. Subsequent
 *            calls use the cached list. It does * not hard‑code th...
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime generation ----------
 * Simple trial‑division sieve up to PRIME_LIMIT.
 * The limit must be large enough to cover all qualifying primes (≤71)
 * and to provide factors for factorisation of p^8−1.  10 000 suffices. */
static bool primes_generated = false;
static uint64_t primes[2000];
static size_t primes_cnt = 0;
#define PRIME_LIMIT 10000

static void generate_primes(void)
{
    if (primes_generated)
        return;
    primes_cnt = 0;
    for (uint64_t n = 2; n <= PRIME_LIMIT; ++n)
    {
        bool is_p = true;
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            uint64_t p = primes[j];
            if (p * p > n)
                break;
            if (n % p == 0)
            {
                is_p = false;
                break;
            }
        }
        if (is_p)
            primes[primes_cnt++] = n;
    }
    primes_generated = true;
}

/* ---------- Add prime factor to a small table ----------
 * `plist`, `exp` and `cnt` hold distinct prime bases and their
 * exponents for the factorisation of a number. */
static void add_factor(uint64_t *plist, int *exp, size_t *cnt,
                       uint64_t prime, int e)
{
    for (size_t i = 0; i < *cnt; ++i)
    {
        if (plist[i] == prime)
        {
            exp[i] += e;
            return;
        }
    }
    plist[*cnt] = prime;
    exp[*cnt] = e;
    ++(*cnt);
}

/* ---------- Test whether a prime qualifies ----------
 * Compute the divisor count of p^8−1 via its factorisation
 * (p−1)(p+1)(p²+1)(p⁴+1).  Return true if the count is <384. */
static bool qualifies(uint64_t p)
{
    /* Factor components of p^8−1. */
    uint64_t f1 = p - 1;
    uint64_t f2 = p + 1;
    uint64_t p2 = p * p;
    uint64_t f3 = p2 + 1;          /* p² + 1 */
    uint64_t p4 = p2 * p2;
    uint64_t f4 = p4 + 1;          /* p⁴ + 1 */

    uint64_t facs[4] = {f1, f2, f3, f4};

    uint64_t plist[64];
    int      exp[64];
    size_t   cnt = 0;

    for (int fi = 0; fi < 4; ++fi)
    {
        uint64_t g = facs[fi];
        for (size_t j = 0; j < primes_cnt; ++j)
        {
            uint64_t q = primes[j];
            if (q * q > g)
                break;
            if (g % q == 0)
            {
                int e = 0;
                while (g % q == 0)
                {
                    g /= q;
                    ++e;
                }
                add_factor(plist, exp, &cnt, q, e);
            }
        }
        if (g > 1)   /* remaining prime factor */
            add_factor(plist, exp, &cnt, g, 1);
    }

    /* Compute number of divisors: product of (exponent+1). */
    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i)
        d *= (uint64_t)(exp[i] + 1);

    return d < 384ULL;
}

/* ---------- Cached sequence ----------
 * The array stores the qualifying primes in order. */
static bool   seq_init_done = false;
static seq_t  qual_primes[200];
static size_t qual_cnt = 0;

static void init_sequence(void)
{
    if (seq_init_done)
        return;
    generate_primes();                     /* primes up to 10 000 */

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (qualifies(p))
            qual_primes[qual_cnt++] = (seq_t)p;
    }
    seq_init_done = true;
}

/* ---------- Sequence function ----------
 * External linkage as required. */
seq_t a(idx_t n)
{
    if (!seq_init_done)
        init_sequence();

    if (n == 0 || n > qual_cnt)
        return (seq_t)0;   /* out of range: return 0 */
    return qual_primes[n - 1];
}
