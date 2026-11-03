/*
 * A375123  Weakly increasing run-leader transformation for standard
 *          compositions.
 * https://oeis.org/A375123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the composition obtained by taking the leaders
 *            of the weakly increasing runs of the composition of n in the
 *            graded reverse‑lexicographic (A066099) order
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Convert a composition to its index in the standard order.  The
   composition is given by parts[0..len-1].  Zero length yields 0. */
static inline seq_t
comp_to_index(const idx_t *parts, size_t len)
{
    if (len == 0)
        return (seq_t)0;

    /* Reverse the parts to obtain the "differences" array */
    uint64_t mask = 0;
    idx_t cum = 0;
    for (size_t i = 0; i < len; ++i) {
        idx_t p = parts[len - 1 - i];
        cum += p;
        /* cum-1 is the bit position that must be set */
        if (cum - 1 < 64)            /* safe for the tested range */
            mask |= ((uint64_t)1) << (cum - 1);
    }
    return (seq_t)mask;
}

/* Convert a non‑negative index n to the composition of n in the
   standard order (graded reverse‑lexicographic).  The parts array
   receives the parts in order and *len receives the number of parts. */
static inline void
get_composition(idx_t n, idx_t *parts, size_t *len)
{
    idx_t bits[64];
    size_t m = 0;
    idx_t tmp = n;

    /* collect positions of 1 bits (least‑significant first) */
    while (tmp) {
        uint8_t pos = (uint8_t)__builtin_ctzll(tmp);
        bits[m++] = (idx_t)pos;
        tmp &= tmp - 1;          /* clear lowest set bit */
    }

    if (m == 0) {                /* empty composition */
        *len = 0;
        return;
    }

    /* differences: first difference of positions with a leading zero */
    idx_t diffs[64];
    diffs[0] = bits[0] + 1;
    for (size_t i = 1; i < m; ++i)
        diffs[i] = bits[i] - bits[i - 1];

    /* parts = reverse(diffs) */
    for (size_t i = 0; i < m; ++i)
        parts[i] = diffs[m - 1 - i];

    *len = m;
}

/* Extract leaders of weakly increasing runs from a composition.
   The leaders array receives the leaders in order and *cnt receives
   their count. */
static inline void
get_leaders(const idx_t *parts, size_t len, idx_t *leaders, size_t *cnt)
{
    if (len == 0) {
        *cnt = 0;
        return;
    }

    size_t l = 0;
    leaders[l++] = parts[0];
    for (size_t i = 1; i < len; ++i) {
        if (parts[i] < parts[i - 1]) /* new run starts when part decreases */
            leaders[l++] = parts[i];
    }
    *cnt = l;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t comp[64];
    size_t comp_len = 0;
    get_composition(n, comp, &comp_len);

    idx_t leaders[64];
    size_t leaders_len = 0;
    get_leaders(comp, comp_len, leaders, &leaders_len);

    return comp_to_index(leaders, leaders_len);
}
