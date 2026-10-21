/*
 * A299987  Lexicographic first sequence of nonnegative integers such that
 *          a(n) + a(n+1) has a digit 7, and no term occurs twice.
 * https://oeis.org/A299987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; for n>0, a(n) is the smallest unused nonnegative
 *            integer such that a(n-1)+a(n) contains the digit 7 (in decimal)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a memoized construction of the lexicographically first
   permutation of the nonnegative integers satisfying the digit‑7 condition. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Check if a value contains the digit 7 in base 10. */
static inline bool has_seven(seq_t x)
{
    if (x == 0) return false;
    while (x) {
        if (x % 10 == 7) return true;
        x /= 10;
    }
    return false;
}

/* Ensure that a dynamic array of seq_t has room for at least 'need' elements. */
static void ensure_seq_cap(idx_t need, seq_t **arr, size_t *cap)
{
    if ((size_t)need < *cap) return;
    size_t newcap = *cap ? *cap : 1;
    while ((size_t)need >= newcap) newcap <<= 1;
    void *tmp = realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) abort();           /* out of memory */
    *arr = (seq_t*)tmp;
    *cap = newcap;
}

/* Ensure that a dynamic usage bitmap has room for at least 'need' entries. */
static void ensure_used_cap(idx_t need, uint8_t **used, size_t *cap)
{
    if ((size_t)need < *cap) return;
    size_t newcap = *cap ? *cap : 1;
    while ((size_t)need >= newcap) newcap <<= 1;
    void *tmp = realloc(*used, newcap);
    if (!tmp) abort();           /* out of memory */
    /* initialize new slots to 0 */
    for (size_t i = *cap; i < newcap; ++i) ((uint8_t*)tmp)[i] = 0;
    *used = (uint8_t*)tmp;
    *cap = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache of computed terms and used bitmap */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint8_t *used = NULL;
    static size_t used_cap = 0;

    /* Build cache up to index n if necessary */
    if (len == 0) {
        /* initialise a(0) = 0 */
        ensure_seq_cap(0, &cache, &cap);
        cache[0] = (seq_t)0;
        len = 1;
        ensure_used_cap(0, &used, &used_cap);
        used[0] = 1;
    }

    while ((size_t)len <= (size_t)n) {
        seq_t prev = cache[len - 1];
        idx_t candidate = 0;
        /* Find smallest unused X such that prev+X has digit 7 */
        for (;;) {
            ensure_used_cap(candidate, &used, &used_cap);
            if (!used[(size_t)candidate]) {
                seq_t sum = prev + (seq_t)candidate;
                if (has_seven(sum)) break;
            }
            ++candidate;
        }
        used[(size_t)candidate] = 1;
        ensure_seq_cap(len, &cache, &cap);
        cache[len] = (seq_t)candidate;
        ++len;
    }

    return cache[n];
}
