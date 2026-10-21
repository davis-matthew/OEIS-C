/*
 * A247625  Knife numbers (second version): n=concat(a,b) is divisible by a+b;
 *          a>0; b must not have leading zeros.
 * https://oeis.org/A247625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Knife numbers (second version):
   A(n) is the nth smallest positive integer that can be written as
   concat(a,b) (i.e., a*10^k + b) with a>0, b≥0, b≠0 for at least one
   valid split, and (a+b) divides concat(a,b).  The function searches
   numbers in ascending order and caches the results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_len.h"

static seq_t *cache = NULL;          /* cache of found terms   */
static size_t cache_size = 0;        /* number of terms cached*/
static size_t cache_cap = 0;         /* allocated capacity    */
static seq_t next_candidate = 10;    /* next integer to test  */

/* Pre‑computed powers of 10 (up to 39 digits for 128‑bit values). */
static seq_t pow10[40] = { 0 };
static bool pow10_inited = false;

static void init_pow10(void)
{
    if (!pow10_inited) {
        pow10[0] = 1;
        for (int i = 1; i < 40; ++i)
            pow10[i] = pow10[i - 1] * 10;
        pow10_inited = true;
    }
}

static void ensure_cache(size_t n)
{
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (n > cache_cap) {
        while (cache_cap < n) cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    size_t target = (size_t)n;
    init_pow10();
    ensure_cache(target);

    while (cache_size < target) {
        seq_t x = next_candidate++;
        int d = digit_len(x);
        bool ok = false;
        for (int k = 1; k < d; ++k) {
            seq_t p = pow10[k];
            seq_t a_part = x / p;
            seq_t b_part = x % p;
            if (b_part == 0)          /* skip splits with b = 0   */
                continue;
            seq_t sum = a_part + b_part;
            if (sum == 0)             /* safety, though impossible */
                continue;
            if (x % sum == 0) {
                ok = true;
                break;
            }
        }
        if (ok) {
            cache[cache_size++] = x;
        }
    }
    return cache[target - 1];
}
