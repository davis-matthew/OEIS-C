/*
 * A331830  Numbers k such that k and k + 1 are both negabinary evil numbers.
 * https://oeis.org/A331830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that k and k+1 are negabinary
 *            evil. Computed via brute‐force search
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9234 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: test if a non‑negative integer is a negabinary evil number.
   An integer is evil if its negabinary representation contains an even
   number of 1's.  The algorithm uses the signed 64‑bit type; the values
   produced by this sequence are far below 2^64, so the cast is safe. */
static inline bool negabinary_is_evil(seq_t x)
{
    /* work in a signed type so we can represent negative intermediates */
    long long n = (long long)x;
    int parity = 0;          /* number of 1's modulo 2 */
    while (n != 0) {
        int bit = (int)(n & 1LL);   /* least significant bit 0 or 1 */
        parity ^= bit;
        n = -(n - bit) / 2;         /* next value in the recursion */
    }
    return parity == 0;
}

/* Dynamically grow the cache that holds the sequence terms. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static seq_t last_checked = 0;  /* largest k that has been examined */

static void ensure_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_cache = mem_resize(seq_t, seq_cache, new_cap);
    seq_cap = new_cap;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, behaviour for n<1 is irrelevant */
        return 0;

    /* Grow the cache until we have n terms. */
    while (seq_len < (size_t)n) {
        seq_t k = last_checked;
        do {
            k++;  /* try the next integer */
        } while (!(negabinary_is_evil(k) && negabinary_is_evil(k + 1)));

        ensure_capacity(seq_len + 1);
        seq_cache[seq_len++] = k;
        last_checked = k;
    }

    return seq_cache[n - 1];
}
