/*
 * A382402  Numbers divisible by the product of their digits (mod 10).
 * https://oeis.org/A382402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 6913, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer n such that 
   n is divisible by (product of its decimal digits) mod 10,
   and the product mod 10 is non‑zero. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/grow_seq.h"

/* Return true if n satisfies the condition, false otherwise. */
static bool satisfies(seq_t n)
{
    seq_t tmp = n;
    uint8_t prod = 1;               /* product modulo 10 */
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 10);
        tmp /= 10;
        prod = (uint8_t)((prod * d) % 10);
    }
    if (prod == 0) return false;    /* excluding numbers with prod mod 10 == 0 */
    return (n % (seq_t)prod) == 0;
}

/* Cache of already computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 1;   /* next number to test */

static void compute_until(size_t n)
{
    while (cache_len < n) {
        if (satisfies(next_candidate)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but guard anyway */
    compute_until((size_t)n);
    return cache[n - 1];
}
