/*
 * A374327  The maximal exponent in the prime factorization of the numbers
 *          whose maximal exponent in their prime factorization is a power of
 *          2.
 * https://oeis.org/A374327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximal exponent of the nth integer m ≥ 2 * whose
 *            maximal prime‑factor exponent is a power of 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Test whether an unsigned integer x is a power of two.  */
static bool is_power_of_two(seq_t x)
{
    /* The exponent values we deal with are very small (≤64), so a cast
       to a native integer is safe. */
    uint64_t ux = (uint64_t)x;
    return ux && !(ux & (ux - 1));
}

/* Smallest‑prime‑factor sieve, grown dynamically.  */
static uint64_t *spf       = NULL;   /* spf[i] = smallest prime factor of i, 0≤i<spf_size */
static idx_t     spf_size  = 0;       /* current size of spf array */
static uint64_t *prime_list = NULL;  /* list of primes found during sieving */
static idx_t     prime_count = 0;     /* number of primes stored */

static void ensure_spf(idx_t limit)
{
    if (limit <= spf_size) return;     /* already OK */

    /* Allocate new space and initialise it to zero.  */
    uint64_t *new_spf = (uint64_t *)realloc(spf, limit * sizeof(uint64_t));
    if (!new_spf) abort();             /* allocation failure */
    spf = new_spf;
    for (idx_t i = 0; i < limit; ++i) spf[i] = 0;

    uint64_t *new_primes = (uint64_t *)realloc(prime_list, limit * sizeof(uint64_t));
    if (!new_primes) abort();
    prime_list = new_primes;
    prime_count = 0;

    for (idx_t i = 2; i < limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_list[prime_count++] = i;
        }
        for (idx_t j = 0; j < prime_count &&
                         prime_list[j] <= spf[i] &&
                         i * prime_list[j] < limit; ++j)
        {
            spf[i * prime_list[j]] = prime_list[j];
        }
    }
    spf_size = limit;
}

/* Return the maximal exponent in the prime factorisation of m.  */
static seq_t max_exponent(idx_t m)
{
    seq_t maxexp = 1;
    while (m > 1) {
        uint64_t p = spf[m];
        seq_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp > maxexp) maxexp = exp;
    }
    return maxexp;
}

/* Cache of sequence values.  */
static seq_t *cache      = NULL;   /* cache[i] = a(i+1) */
static size_t cache_cap  = 0;
static size_t cache_size = 0;
static idx_t  last_m     = 1;      /* last integer examined (starts at 1) */

/* Append a value to the cache, enlarging it if necessary.  */
static void append_cache(seq_t val)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Compute terms until we have reached index n.  */
    while (cache_size < (size_t)n) {
        ++last_m;                                         /* next m to test */
        ensure_spf(last_m + 1);                           /* spf up to last_m */
        seq_t exp = max_exponent(last_m);                 /* maximal exponent */
        if (is_power_of_two(exp)) {
            append_cache(exp);
        }
    }
    return cache[(size_t)n - 1];
}
