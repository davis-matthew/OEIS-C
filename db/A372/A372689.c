/*
 * A372689  Positive integers whose binary indices (positions of ones in
 *          reversed binary expansion) sum to a prime number.
 * https://oeis.org/A372689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth positive integer whose binary indices sum to a prime number
   (OEIS A372689).  */

#include <stdbool.h>
#include <stdlib.h>

/* ---------- small‑prime test for values ≤ 8500 (sum of positions) ---------- */
static bool is_prime_sum(uint64_t s)
{
    if (s < 2) return false;
    for (uint64_t i = 2; i * i <= s; ++i)
        if (s % i == 0) return false;
    return true;
}

/* ---------- cache for computed terms ---------- */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of stored terms   */
static size_t cache_cap  = 0;   /* allocated capacity       */
static seq_t next_candidate = 2;/* first number to test     */

/* Ensure the cache can hold at least N terms. */
static void ensure_cache_capacity(size_t N)
{
    if (cache_cap >= N) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < N) new_cap = N;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Test whether a number satisfies the property. */
static bool has_prime_binary_index_sum(seq_t value)
{
    uint64_t sum = 0;
    unsigned int pos = 1;
    seq_t x = value;
    while (x != 0) {
        if (x & 1) sum += pos;
        x >>= 1;
        ++pos;
    }
    return is_prime_sum(sum);
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    /* n is 1‑based. For n==0 the function never calls a(0) internally,
       but return 0 for safety. */
    if (n == 0) return 0;

    /* Grow the cache until it contains the nth element. */
    while (cache_size < (size_t)n) {
        /* Find next number that satisfies the property. */
        while (!has_prime_binary_index_sum(next_candidate))
            ++next_candidate;

        /* Store it. */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size] = next_candidate;
        ++cache_size;

        /* Advance candidate for next search. */
        ++next_candidate;
    }

    return cache[n - 1];
}
