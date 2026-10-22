/*
 * A308560  Numbers divisible by the LCM of all sums of any two digits (at
 *          distinct positions).
 * https://oeis.org/A308560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer divisible by the LCM of all sums of
 *            two distinct digits
 * Range:     u8 n <= 42, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Test whether x satisfies the A308560 property. */
static bool satisfies(seq_t x)
{
    /* Extract digits into a small array. */
    uint8_t digits[256];
    size_t dcnt = 0;
    seq_t tmp = x;

    if (tmp == (seq_t)0) {
        digits[0] = 0;
        dcnt = 1;
    } else {
        while (tmp != (seq_t)0) {
            seq_t d = tmp % (seq_t)10;
            digits[dcnt++] = (uint8_t)d;
            tmp = tmp / (seq_t)10;
        }
    }

    /* Count zeros and reject if more than one zero. */
    size_t zero_cnt = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if (digits[i] == 0)
            ++zero_cnt;
        if (zero_cnt > 1)
            return false;
    }

    /* One‑digit numbers always satisfy the condition. */
    if (dcnt < 2)
        return true;

    /* Compute LCM of all pairwise digit sums. */
    seq_t lcm_val = (seq_t)1;
    for (size_t i = 0; i < dcnt; ++i) {
        for (size_t j = i + 1; j < dcnt; ++j) {
            uint8_t sum = digits[i] + digits[j];          /* at most 18 */
            seq_t sum_seq = (seq_t)sum;
            seq_t g = gcd(lcm_val, sum_seq);
            lcm_val = (lcm_val / g) * sum_seq;
        }
    }

    return (x % lcm_val) == (seq_t)0;
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* Undefined for n < 1, behaviour irrelevant */

    /* Static storage for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_candidate = (idx_t)1;

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < n) {
        /* Find next number satisfying the condition. */
        while (!satisfies((seq_t)next_candidate))
            ++next_candidate;

        /* Store the found term. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
