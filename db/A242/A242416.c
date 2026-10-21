/*
 * A242416  Numbers whose prime factorization viewed as a tuple of nonzero
 *          powers is not palindromic.
 * https://oeis.org/A242416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k ≥ 2 whose prime‑factor exponent tuple
 *            * is not a palindrome (i.e., the tuple of positive exponents in
 *            * k = p1^e1·p2^e2·…·pr^er is not equal to its reverse). * * The
 *            algorithm simply scans integers in increasing order, tests the *
 *            condition by trial division, and caches...
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for already computed terms. */
static seq_t *cache = NULL;        /* array of terms */
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* allocated capacity */

/* State for incremental generation. */
static uint64_t next_candidate = 2;   /* next integer to test */
static idx_t   generated = 0;         /* number of terms generated so far */

/* Ensure the cache has enough room for at least `need` elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p) abort();   /* out of memory */
    cache = p;
    cache_cap = new_cap;
}

/* Test whether `num` (uint64_t) has a non‑palindromic exponent tuple.
 * Returns true  if the tuple is NOT a palindrome (i.e. the number
 * belongs to the sequence), false otherwise. */
static bool valid_candidate(uint64_t num)
{
    /* Factor `num` and record exponents. */
    uint64_t n = num;
    int exps[64];
    size_t count = 0;

    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exps[count++] = e;
        }
    }
    if (n > 1) {           /* remaining prime factor with exponent 1 */
        exps[count++] = 1;
    }

    if (count <= 1) return false;   /* single prime power => palindrome */

    /* Check for palindromic sequence of exponents. */
    for (size_t i = 0; i < count / 2; ++i) {
        if (exps[i] != exps[count - 1 - i]) return true;   /* not palindrome */
    }
    return false;   /* palindrome */
}

/* Append `value` to the cache, expanding if needed. */
static void append_cache(seq_t value)
{
    grow_cache(cache_len + 1);
    cache[cache_len++] = value;
}

/* Generate terms until at least `n` terms are stored. */
static void ensure_n_terms(idx_t n)
{
    while (generated < n) {
        if (valid_candidate(next_candidate)) {
            append_cache((seq_t)next_candidate);
            ++generated;
        }
        ++next_candidate;
    }
}

/* The required external function. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Beyond that, behavior is
     * unspecified, but we simply return 0. */
    if (n == 0) return 0;

    ensure_n_terms(n);
    return cache[n - 1];
}
