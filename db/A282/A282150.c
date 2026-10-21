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
 * Helpers:   satisfies_base9_fulcrum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A282150 by brute‑force search in base 9 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/satisfies_base9_fulcrum.h"

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
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_cap < n) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
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
