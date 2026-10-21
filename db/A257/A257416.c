/*
 * A257416  Values of n such that there are exactly 9 solutions to x^2 - y^2 =
 *          n with x > y >= 0.
 * https://oeis.org/A257416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 427, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n with exactly nine representations as x² – y² = n,  x > y ≥ 0
   The function counts the number of solutions using the divisor‑count method.
   For an odd n the number of solutions equals ⌊τ(n)/2⌋;
   for an even n = 2ᵏ·m (m odd, k ≥ 2) it equals ⌊τ(n/4)/2⌋.
   The function generates terms on demand, caching them in a vector (C++)
   or a reallocatable array (C).  The required primes are precomputed once
   with a simple sieve up to 1 000 000.  The algorithm is O(n log log n) for
   generating the first n terms.  It works for both 64‑bit builds and
   arbitrary‑precision builds that expose a typdef seq_t.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----------------------------------------------------------------------
   Prime sieve (once per program).  1 000 000 is enough for all terms
   up to at least the first 10 000.  The primes array is stored as a
   plain C array in both C and C++ to avoid constructor problems.
   ---------------------------------------------------------------------- */
static const uint32_t PRIME_LIMIT = 1000000;
static uint32_t *prime_list = NULL;   /* NULL indicates not yet built */
static size_t prime_cnt   = 0;

static void init_primes(void)
{
    if (prime_list != NULL) return;          /* already built */

    /* Sieve of Eratosthenes */
    uint8_t *sieve = NULL;
    size_t i, j;

    sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    if (!sieve) { exit(EXIT_FAILURE); }

    for (i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!sieve[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;

    /* Count primes */
    prime_cnt = 0;
    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) ++prime_cnt;

    /* Allocate and fill the list */
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) { free(sieve); exit(EXIT_FAILURE); }

    size_t idx = 0;
    for (i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) prime_list[idx++] = (uint32_t)i;

    free(sieve);
}

/* ----------------------------------------------------------------------
   Return the number of (unordered) factor pairs of n, i.e. τ(n),
   where τ is the divisor function.  The factorisation is done by
   trial division using the precomputed primes.  The function works
   for n up to 2⁶⁴.  It accepts n as uint64_t to avoid overflow.
   ---------------------------------------------------------------------- */
static unsigned long divisor_count_uint64(uint64_t n)
{
    if (n < 2) return 1;            /* τ(1)=1 */
    unsigned long result = 1;
    init_primes();

    for (size_t pi = 0; pi < prime_cnt; ++pi)
    {
        uint32_t p = prime_list[pi];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0)
        {
            unsigned exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (n > 1)                         /* remaining prime factor */
        result *= 2;
    return result;
}

/* ----------------------------------------------------------------------
   Count the number of solutions to x² – y² = n with x > y ≥ 0.
   The formula:  if n odd -> τ(n) / 2; if n even with exponent k of 2
   equal 1 -> 0; else τ(n/4)/2.  Always return an unsigned long
   (fits in 64 bits when n ≤ 2⁶⁴).
   ---------------------------------------------------------------------- */
static unsigned long solutions_count_uint64(uint64_t n)
{
    unsigned k = __builtin_ctzll(n);   /* exponent of 2 in n */
    if (k == 0)                       /* odd n */
        return divisor_count_uint64(n) / 2;
    if (k == 1)                       /* 2 * odd: no solutions */
        return 0;
    /* k >= 2, n divisible by 4 */
    uint64_t m = n >> 2;              /* n / 4 */
    return divisor_count_uint64(m) / 2;
}

/* ----------------------------------------------------------------------
   Cached sequence storage.  Two implementations: one for C++ (vector)
   and one for C (reallocatable array).  The code is wrapped in
   #ifdef __cplusplus so that C compilation uses the array version.
   ---------------------------------------------------------------------- */
#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache_vals;
static uint64_t last_checked = 0;      /* last integer examined */
#else
static seq_t *cache_vals = NULL;
static size_t cache_size  = 0;
static size_t cache_cap   = 0;
static uint64_t last_checked = 0;
#endif

/* Ensure the C array has enough capacity */
#ifndef __cplusplus
static void cache_ensure_size(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!tmp) { free(cache_vals); exit(EXIT_FAILURE); }
    cache_vals = tmp;
    cache_cap  = new_cap;
}
#endif

/* ----------------------------------------------------------------------
   Main required function: a(idx_t n) returns the nth term (1‑based).
   The function generates terms lazily, caching them up to n
   if never seen before.  Undefined for n == 0.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

#ifdef __cplusplus
    /* C++ implementation using std::vector */
    while (cache_vals.size() < (size_t)n)
    {
        ++last_checked;
        if (solutions_count_uint64(last_checked) == 9)
            cache_vals.push_back((seq_t)last_checked);
    }
    return cache_vals[(size_t)n - 1];
#else
    /* C implementation using reallocatable array */
    while (cache_size < (size_t)n)
    {
        ++last_checked;
        if (solutions_count_uint64(last_checked) == 9)
        {
            cache_ensure_size(cache_size + 1);
            cache_vals[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache_vals[(size_t)n - 1];
#endif
}
