/*
 * A271984  Numbers n such that the denominator of the sum of the reciprocals
 *          of the exponents in the binary expansion of 2n is not equal to
 *          their LCM. That is, A271410(n) != A116417(n).
 * https://oeis.org/A271984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS formula: a(n) is the nth integer m (m ≥ 1) such that the denominator
 * of Σ 1/e for e being the positions of 1‐bits in the binary expansion of
 * 2m is different from the LCM of those exponents.
 *
 * The function computes the sequence by brute force search with a cache.
 * The algorithm uses only integer arithmetic and the provided `gcd`
 * helper for seq_t types.  It works for all supported seq_t sizes
 * (uint8_t … arbitrary precision) provided `gcd.h` is available.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Cache for generated sequence values */
static seq_t *seq_cache = NULL;  /* cached values */
static size_t  seq_len   = 0;    /* number of cached values   */
static size_t  seq_cap   = 0;    /* capacity of seq_cache     */

/* Next integer to test (as idx_t to avoid overflow of seq_t) */
static idx_t next_candidate = 1;

/* Ensure cache has space for at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *newp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
    if (!newp) abort();
    seq_cache = newp;
    seq_cap   = newcap;
}

/* Test whether a given m satisfies the condition. */
static bool good_candidate(seq_t m)
{
    seq_t t = m << 1;          /* t = 2*m */
    seq_t lcm_val = 1;
    seq_t sum_num = 0, sum_den = 1;

    uint64_t pos = 0;
    while (t)
    {
        if (t & 1)
        {
            seq_t e = (seq_t)pos;

            /* Update LCM of the exponents */
            seq_t g_l = gcd(lcm_val, e);
            lcm_val = (lcm_val / g_l) * e;

            /* Update the sum of reciprocals 1/e */
            seq_t g_d = gcd(sum_den, e);
            seq_t tmp = e / g_d;
            seq_t new_den = sum_den * tmp;
            seq_t num_add = sum_den / g_d;
            seq_t new_num = sum_num * tmp + num_add;
            seq_t g_r = gcd(new_num, new_den);
            sum_num = new_num / g_r;
            sum_den = new_den / g_r;
        }
        t >>= 1;
        ++pos;
    }
    return lcm_val != sum_den;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0 */

    /* Generate terms until we have at least n values. */
    while (seq_len < n)
    {
        seq_t candidate = (seq_t)next_candidate;
        if (good_candidate(candidate))
        {
            grow_cache(seq_len + 1);
            seq_cache[seq_len++] = candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
