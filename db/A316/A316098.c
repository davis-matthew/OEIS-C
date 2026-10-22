/*
 * A316098  Abundant numbers that differ from the next abundant number by 4.
 * https://oeis.org/A316098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 4975, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A316098: abundant numbers whose next abundant number is exactly
   4 larger.  The function scans natural numbers, remembers the most recent
   abundant number and, whenever the current abundant number is 4 more
   than the stored one, records the stored number.  The list of such
   numbers is cached so that subsequent calls with larger indices are
   fast. */
/* The function uses only integer operations and standard library calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static uint32_t *primes = NULL;
static size_t primes_count = 0;

/* Initialise a table of primes up to a fixed limit (one million). */
static void init_primes(void)
{
    if (primes != NULL)
        return;                           /* already initialised */

    const int LIMIT = 1000000;
    unsigned char *sieve = (unsigned char *)calloc((size_t)LIMIT + 1, 1);
    if (!sieve) abort();

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++primes_count;

    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    if (!primes) abort();

    size_t pos = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            primes[pos++] = (uint32_t)i;

    free(sieve);
}

/* Sum of divisors of n (including n itself) using the pre‑computed primes. */
static uint64_t sigma_uint64(uint64_t n)
{
    uint64_t result = 1;
    uint64_t tmp = n;

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp)
            break;

        if (tmp % p == 0) {
            uint64_t term = 1;
            uint64_t power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= p;
                term += power;
            }
            result *= term;
            if (tmp == 1)
                break;
        }
    }

    if (tmp > 1)          /* remaining prime factor */
        result *= (tmp + 1);

    return result;
}

/* Return the n-th term of A316098. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* sequence offset starts at 1 */

    /* Static cache to remember the list of terms found so far. */
    static seq_t *cache = NULL;       /* array of terms */
    static size_t cache_len = 0;      /* number of terms stored */

    static uint64_t prev_abundant = 0;/* most recent abundant number found */
    static uint64_t cur = 1;          /* current integer being examined */

    init_primes();                    /* ensure we have the prime list */

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t sigma = sigma_uint64(cur);
        bool abundant = sigma > 2 * cur;    /* sigma(cur) > 2*cur */

        if (abundant) {
            if (prev_abundant != 0 && cur == prev_abundant + 4) {
                /* Store prev_abundant in the cache. */
                seq_t *tmp = (seq_t *)realloc(cache,
                                              (cache_len + 1) * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache[cache_len++] = (seq_t)prev_abundant;
            }
            prev_abundant = cur;
        }
        ++cur;
    }

    return cache[n - 1];
}
