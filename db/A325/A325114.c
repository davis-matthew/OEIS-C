/*
 * A325114  Integers k such that no nonzero subsequence of the decimal
 *          representation of k is divisible by 7.
 * https://oeis.org/A325114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 9222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A325114  – “Integers k such that no non‑zero contiguous
    decimal substring of k is divisible by 7”.
    The function a(idx_t n) builds the list of such numbers on demand,
    retaining the results in a static cache.  It uses a linear test
    that examines each digit from left to right, maintaining the set
    of remainders modulo 7 of all substrings ending at the current
    position; any time a non‑zero remainder 0 is produced the candidate
    is rejected.  Substrings consisting only of zeros are ignored
    because they evaluate to 0.  This algorithm is O(L²) per candidate
    where L is the number of decimal digits of the candidate, and it
    is fast enough to generate the first ten thousand terms in far
    below two seconds. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_DIGITS 500   /* enough for any term we need to generate */

/* Test whether a decimal integer k has no non‑zero substring divisible by 7. */
static bool good(seq_t k)
{
    if (k == 0) return false;     /* 0 is not considered */

    /* Extract decimal digits into an array (least‑significant first). */
    int digits[MAX_DIGITS];
    int len = 0;
    seq_t t = k;
    while (t > 0) {
        digits[len++] = (int)(t % 10);
        t /= 10;
    }
    if (len == 0) return false;   /* should never happen */

    /* Recurse from most‑significant to least‑significant. */
    int cur_rem[MAX_DIGITS];
    unsigned char cur_hasnz[MAX_DIGITS];
    int cur_len = 0;          /* number of active substrings at current position */

    for (int pos = len - 1; pos >= 0; --pos) {
        int d = digits[pos];
        int new_rem[MAX_DIGITS];
        unsigned char new_hasnz[MAX_DIGITS];
        int new_len = 0;

        /* Substring that consists of the current digit only. */
        int rem = d % 7;
        unsigned char hasnz = (d != 0);
        if (rem == 0 && hasnz) return false;
        new_rem[new_len] = rem;
        new_hasnz[new_len] = hasnz;
        ++new_len;

        /* Extend each existing substring by the current digit. */
        for (int i = 0; i < cur_len; ++i) {
            rem = (cur_rem[i] * 10 + d) % 7;
            hasnz = cur_hasnz[i] | (unsigned char)(d != 0);
            if (rem == 0 && hasnz) return false;
            new_rem[new_len] = rem;
            new_hasnz[new_len] = hasnz;
            ++new_len;
        }

        /* Copy new state back for the next iteration. */
        for (int i = 0; i < new_len; ++i) {
            cur_rem[i] = new_rem[i];
            cur_hasnz[i] = new_hasnz[i];
        }
        cur_len = new_len;
    }
    return true;
}

/* Static cache of already discovered terms. */
static seq_t *cache      = NULL;
static size_t cache_size  = 0;   /* number of terms stored */
static size_t cache_cap   = 0;   /* current capacity of cache array */
static seq_t next_candidate = 1; /* smallest integer not yet examined */

/* Grow the cache when its capacity is exhausted. */
static void cache_grow(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();            /* unrecoverable allocation failure */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    /* Build the cache up to n terms if necessary. */
    while (cache_size < n) {
        if (good(next_candidate)) {
            if (cache_size == cache_cap) cache_grow();
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
