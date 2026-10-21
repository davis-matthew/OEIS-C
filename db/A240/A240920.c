/*
 * A240920  Prime numbers that occur as divisors of numbers of the form m^2 +
 *          5.
 * https://oeis.org/A240920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   This function implements OEIS A240920 – the primes that * divide
 *            some number of the form m² + 5. A prime p is contained in * the
 *            sequence iff p = 2 or p = 5 or p ≡ 1,3,5,7,9 (mod 20). The *
 *            function returns the n-th such prime (a(1) = 2)
 * Range:     u8 n <= 26, u16 n <= 3241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------  Include headers  -------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* -----------------  Cache for sequence terms  ----------------- */

/* Static array holding the sequence values that have already been
 * generated.  The array is grown as needed. */
static seq_t *cache      = NULL;   /* array of computed terms   */
static size_t cache_len  = 0;      /* number of stored terms    */
static size_t cache_cap  = 0;      /* allocated capacity        */

/* Ensure that the cache can hold at least `needed` elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap > 0 ? cache_cap * 2 : 1);
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                 /* simple error handling */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void append_to_cache(seq_t val)
{
    ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* -----------------  Prime generation  ----------------- */

/* Static array holding all primes that have been generated so far.
 * The array is incrementally enlarged as needed. */
static uint64_t *prime_list = NULL;   /* array of primes   */
static size_t    prime_cnt  = 0;     /* number of primes  */
static size_t    prime_cap  = 0;     /* allocated capacity */
static uint64_t  next_candidate = 2; /* next odd candidate after 2 */

/* Ensure the prime list has the given capacity. */
static void ensure_prime_list_cap(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = (prime_cap > 0 ? prime_cap * 2 : 1);
    while (new_cap < needed)
        new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp)
        exit(1);
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Test whether a candidate is prime using the list of already found
 * primes.  This works for any candidate that fits in uint64_t. */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Generate the next prime > (or =) the last prime seen.
 * The first call returns 2. */
static uint64_t next_prime(void)
{
    if (prime_cnt == 0)              /* First prime     */
    {
        if (prime_cap == 0)
            ensure_prime_list_cap(1);
        prime_list[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;          /* Next odd candidate */
        return 2;
    }

    /* Ensure the candidate is odd and not below the previous prime. */
    if (next_candidate <= prime_list[prime_cnt - 1])
        next_candidate = prime_list[prime_cnt - 1] + 1;
    if (next_candidate <= 2)
        next_candidate = 3;
    else if ((next_candidate & 1) == 0)
        next_candidate++;

    /* Find next prime. */
    while (true)
    {
        if (is_prime_candidate(next_candidate))
        {
            if (prime_cnt == prime_cap)
                ensure_prime_list_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = next_candidate;
            uint64_t prime = next_candidate;
            next_candidate += 2;       /* Next odd candidate */
            return prime;
        }
        next_candidate += 2;           /* Skip even numbers */
    }
}

/* -----------------  Sequence term predicate  ----------------- */

/* A prime p is in the sequence if and only if it is 2, 5, or
 * congruent to 1,3,5,7,9 modulo 20. */
static bool in_sequence(uint64_t p)
{
    if (p == 2 || p == 5)
        return true;
    uint64_t r = p % 20;
    return (r == 1 || r == 3 || r == 5 || r == 7 || r == 9);
}

/* -----------------  Main sequence function  ----------------- */

/* Return the n-th term of the sequence (a(1)=2).  The function
 * generates terms on demand and caches them, so it works for
 * arbitrary call order.  The implementation is O(n) with small
 * constant factors. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for offset 0 */

    /* Generate terms until we have at least n cached. */
    while (cache_len < (size_t)n)
    {
        uint64_t p = next_prime();      /* get next prime */
        if (in_sequence(p))
            append_to_cache((seq_t)p); /* cast to sequence type */
    }

    return cache[(size_t)(n - 1)];
}
