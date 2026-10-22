/*
 * A394424  The number of exponential divisors of the exponentially odd
 *          numbers.
 * https://oeis.org/A394424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A049419(A268335(n)) – number of exponential divisors of
 *            the n‑th exponentially odd number
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- small divisor count for integers <= 64 --------- */
static inline int divisor_count_small(int n)
{
    int c = 0;
    for (int d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (d * d == n)
                ++c;
            else
                c += 2;
        }
    }
    return c;
}

/* --------- primes ------------------------------------------ */
#define PRIME_LIMIT 200000
static uint32_t primes[PRIME_LIMIT + 1];
static size_t primes_cnt = 0;
static bool primes_prepared = false;

static void build_primes(void)
{
    if (primes_prepared)
        return;
    static bool sieve[PRIME_LIMIT + 1];
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
    primes_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            primes[primes_cnt++] = (uint32_t)i;
    primes_prepared = true;
}

/* --------- exponential divisor count ----------------------- */
static inline seq_t exp_div_count(uint64_t val)
{
    seq_t exp_div = 1;
    uint64_t temp = val;
    for (size_t i = 0; i < primes_cnt &&
                     (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i)
    {
        uint32_t p = primes[i];
        int e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }
        if (e > 0)
        {
            if ((e & 1) == 0)          /* even exponent → not exponentially odd */
                return 0;
            int dcnt = divisor_count_small(e);
            exp_div *= (seq_t)dcnt;
        }
    }
    /* Remaining factor (if any) has exponent 1 → odd, multiplier 1 */
    return exp_div;
}

/* --------- cache for sequence values ----------------------- */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;
static size_t a_cache_cap = 0;
static uint64_t last_checked = 0;

/* --------- sequence function ------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 0, safe fallback */

    if (!a_cache)
    {
        build_primes();
        a_cache_cap = 1024;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache)
            abort();
        a_cache_len = 0;
        last_checked = 0;
    }

    while (a_cache_len < (size_t)n)
    {
        ++last_checked;                /* next integer to test */
        seq_t val = exp_div_count(last_checked);
        if (val != 0)                  /* exponentially odd number */
        {
            if (a_cache_len + 1 > a_cache_cap)
            {
                a_cache_cap *= 2;
                a_cache = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
                if (!a_cache)
                    abort();
            }
            a_cache[a_cache_len++] = val;
        }
    }

    return a_cache[n - 1];             /* 0‑based array */
}
