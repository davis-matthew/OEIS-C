/*
 * A296714  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9831 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296714: numbers whose decimal digits have fewer rises than falls. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* return 1 if the number of rises is strictly less than the number of falls,
   according to its decimal digits read most‑to‑least.        */
static inline int has_more_falls(seq_t x)
{
    if (x == 0) return 0;          /* single digit numbers have 0 rises/falls */
    uint8_t dig[128];
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        dig[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        uint8_t higher = dig[i];   /* more significant digit */
        uint8_t lower  = dig[i - 1]; /* less significant digit */
        if (higher > lower) ++falls;
        else if (higher < lower) ++rises;
    }
    return rises < falls;
}

/* count of terms already generated and the cached values */
static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t count = 0;
/* next integer to examine – the first candidate is 10 */
static seq_t next = 10;

/* append a new term into the cache, growing the array if needed */
static inline void push_value(seq_t val)
{
    if (count >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cap = new_cap;
    }
    cache[count++] = val;
}

/* public entry point used by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */
    while (count < n) {
        if (has_more_falls(next)) push_value(next);
        ++next;
    }
    return cache[n - 1];
}
