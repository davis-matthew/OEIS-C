/*
 * A308810  Odd positive integers k such that the repetend of the binary
 *          representation of 1/k contains more 1's than 0's.
 * https://oeis.org/A308810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): nth odd positive integer k such that the binary
 * repetend of 1/k contains more 1's than 0's.  Uses a simple
 * search with memoisation. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Test whether an odd integer k>1 has more 1's than 0's in the binary
 * repetend of 1/k.  Assumes k is odd and k>1. */
static bool good_k(idx_t k)
{
    /* binary long division: start with remainder 1 (mod k). */
    seq_t rem = 1 % (seq_t)k;
    seq_t ones = 0, zeros = 0;
    const seq_t kseq = (seq_t)k;

    /* The period ends when the remainder equals the starting remainder (1). */
    while (true) {
        rem = rem * 2;
        seq_t bit = rem / kseq;          /* 0 or 1 */
        if (bit)
            ++ones;
        else
            ++zeros;
        rem = rem % kseq;
        if (rem == (seq_t)1)
            break;
    }
    return ones > zeros;
}

/* Static cache for already found terms. */
static seq_t *cache      = NULL;   /* array of terms */
static size_t capacity   = 0;      /* allocated slots */
static size_t found_cnt  = 0;      /* number of terms stored */
static idx_t  next_k     = 3;      /* next odd number to test (skipping 1) */

/* Ensure the cache has at least min_cap elements. */
static void ensure_capacity(size_t min_cap)
{
    if (capacity >= min_cap)
        return;
    size_t new_cap = capacity ? capacity * 2 : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In production code we would handle the error; here we abort. */
        abort();
    }
    cache = new_ptr;
    capacity = new_cap;
}

/* Return the nth term of the sequence (n>=1).  a(0) is defined as 0. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1 */
        return 0;

    /* If we already have enough terms, return from cache. */
    if (found_cnt >= n)
        return cache[n - 1];

    /* Otherwise, extend the cache until we have n terms. */
    ensure_capacity((size_t)n);

    while (found_cnt < n) {
        /* Skip non‑positive, even, or the value 1. */
        while (next_k <= 1 || (next_k & 1) == 0)
            ++next_k;

        /* Test the candidate. */
        if (good_k(next_k))
            cache[found_cnt++] = (seq_t)next_k;

        ++next_k;
    }
    return cache[n - 1];
}
