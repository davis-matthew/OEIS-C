/*
 * A282150  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j..k}{(i-j+1)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 9.
 * https://oeis.org/A282150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2959, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9199 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A282150 by brute‑force search in base 9 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether x satisfies the fulcrum condition in base 9 */
static bool satisfies_base9_fulcrum(uint64_t x)
{
    uint8_t rev[32];
    int rev_len = 0;
    uint64_t t = x;
    if (t == 0) return false;
    while (t > 0) {
        rev[rev_len++] = t % 9;
        t /= 9;
    }
    if (rev_len < 2) return false;              /* must have at least two digits */

    uint8_t d[32];
    for (int i = 0; i < rev_len; ++i)
        d[i] = rev[rev_len - 1 - i];           /* most significant first */

    int len = rev_len;
    for (int j = 2; j <= len; ++j) {            /* j is the first digit on the right side */
        unsigned long long left  = 0;
        unsigned long long right = 0;
        int j0 = j - 1;                        /* zero‑based index of the right part start */

        for (int i = 0; i < j0; ++i)
            left += (unsigned long long)(j0 - i) * d[i];

        for (int i = j0; i < len; ++i)
            right += (unsigned long long)(i - j0 + 1) * d[i];

        if (left == right) return true;
    }
    return false;
}

/* cache for previously found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
/* next decimal number to be tested */
static uint64_t next_candidate = 10;

/* ensure the cache can hold at least n elements */
static void grow_cache(size_t n)
{
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = mem_new(seq_t, cache_cap);
    }
    while (cache_cap < n) {
        cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* OEIS definition: a(n) is the nth number whose base‑9 representation
 * satisfies the fulcrum condition described above. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n<1, return 0 */

    if ((size_t)n > cache_size) {
        grow_cache((size_t)n);
        while (cache_size < (size_t)n) {
            if (satisfies_base9_fulcrum(next_candidate))
                cache[cache_size++] = (seq_t)next_candidate;
            ++next_candidate;
        }
    }
    return cache[(size_t)n - 1];
}
