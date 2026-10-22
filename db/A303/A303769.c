/*
 * A303769  a(0) = 0, a(n+1) is either the largest number obtained from a(n)
 *          by toggling a single 1-bit off (to 0) if no such number is yet in
 *          the sequence, otherwise the least number not yet in sequence that
 *          can be obtained from a(n) by toggling a single 0-bit on (to 1). In
 *          both cases the bit to be toggled...
 * https://oeis.org/A303769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined by the OEIS specification – a recursive *
 *            construction that toggles a single bit of the previous term. * *
 *            a(0) = 0 * a(n+1) = * if there exists an unused number obtained
 *            by turning off a 1-bit of a(n), * choose the largest such number
 *            (rightmost off bit) * else * choose the small...
 * Range:     u8 n <= 156, u16 n <= 9700, u32 n <= 9700, u64 n <= 9700,
 *            u128 n <= 9700, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for malloc, realloc */
#include <stdbool.h>         /* for bool */
#include "../utils/bitlen_seq_t.h"

static seq_t *seq_vals = NULL;      /* memoised sequence values */
static size_t seq_capacity = 0;     /* allocated capacity of seq_vals  */
static size_t seq_len = 0;          /* number of values computed so far */

/* dynamic array helpers --------------------------------------------------- */
static void seq_grow(void)
{
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_capacity = new_cap;
}

static void seq_push(seq_t v)
{
    if (seq_len >= seq_capacity)
        seq_grow();
    seq_vals[seq_len++] = v;
}

/* linear search for previously seen value -------------------------------- */
static inline bool seen(seq_t x)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq_vals[i] == x)
            return true;
    return false;
}

/* compute next term of the sequence -------------------------------------- */
static seq_t next_term(seq_t cur)
{
    /* try turning off a 1‑bit – choose the rightmost eligible bit
     * (largest result among unseen candidates) */
    int bitlen = (int)bitlen_seq_t(cur);
    int bitwidth = (int)(sizeof(seq_t) * 8);
    seq_t best_off = 0;
    bool found_off = false;

    for (int b = 0; b < bitlen && b < bitwidth; ++b) {
        seq_t mask = ((seq_t)1 << b);
        if (cur & mask) {
            seq_t cand = cur - mask;
            if (!seen(cand)) {
                if (!found_off || cand > best_off) {
                    best_off = cand;
                    found_off = true;
                }
            }
        }
    }
    if (found_off)
        return best_off;

    /* otherwise try turning on a 0‑bit – choose the rightmost eligible bit
     * (smallest result among unseen candidates) */
    int maxbit = bitlen + 1;
    if (maxbit > bitwidth)
        maxbit = bitwidth - 1;      /* avoid undefined shift on very wide types */

    for (int b = 0; b <= maxbit; ++b) {
        seq_t mask = ((seq_t)1 << b);
        if (!(cur & mask)) {
            seq_t cand = cur + mask;
            if (!seen(cand))
                return cand;
        }
    }

    /* Theoretical guarantee: a candidate will always be found. */
    return cur; /* unreachable */
}

/* public API -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (seq_len == 0) {          /* first call – seed the sequence */
        seq_push((seq_t)0);
    }

    while (seq_len <= (size_t)n) {
        seq_t next = next_term(seq_vals[seq_len - 1]);
        seq_push(next);
    }

    return seq_vals[(size_t)n];
}
