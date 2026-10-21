/*
 * A290388  Increasing sequence where a(n) is the smallest integer not yet in
 *          the sequence with no digits shared with the terms a(n-1) and
 *          a(n-2), no repeated digits, and no 0-digit allowed.
 * https://oeis.org/A290388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: pick the smallest integer > previous term that has distinct non‑zero digits
   and shares no digit with the two preceding terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Helpers: digit mask calculator without repeated digits, no zero
 *  Returns true if value passes all checks and mask is set to its digit mask.
 */
static inline bool valid_candidate(uint64_t v, seq_t forbidden, seq_t *mask_out)
{
    seq_t mask = 0;
    while (v > 0)
    {
        unsigned int d = v % 10;
        if (d == 0)
            return false;               /* zero digit not allowed */
        unsigned int bit = 1u << (d - 1);
        if (mask & bit)
            return false;               /* repeated digit inside the number */
        mask |= bit;
        v /= 10;
    }
    if (mask & forbidden)
        return false;                   /* shares a forbidden digit */
    if (mask_out)
        *mask_out = mask;
    return true;
}
/* ------------------------------------------------------------ */

/* Sequence state: static, built incrementally on demand. */
static seq_t  *seq_vals  = NULL;   /* computed sequence values  */
static seq_t  *seq_masks = NULL;   /* digit‑mask of each value   */
static size_t  seq_len   = 0;       /* number of terms computed  */
static size_t  seq_cap   = 0;       /* allocated capacity        */

/* Expand storage if necessary.  New capacity is doubled. */
static void ensure_capacity(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_vals = (seq_t*)realloc(seq_vals,  new_cap * sizeof(seq_t));
    seq_masks = (seq_t*)realloc(seq_masks, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n <= 0, but harmless */

    /* Build the sequence up to term n if necessary. */
    while (seq_len < (size_t)n)
    {
        if (seq_len == 0)               /* a(1) = 1 */
        {
            ensure_capacity(1);
            seq_vals[0]  = (seq_t)1;
            seq_masks[0] = (seq_t)1;    /* digit 1 -> mask 0b1 */
            seq_len = 1;
            continue;
        }

        /* Determine forbidden digits: union of digits of the two previous terms. */
        seq_t mask_prev  = seq_masks[seq_len - 1];
        seq_t mask_prev2 = (seq_len >= 2) ? seq_masks[seq_len - 2] : (seq_t)0;
        seq_t forbidden  = mask_prev | mask_prev2;

        /* Previous term value. */
        seq_t last_val = seq_vals[seq_len - 1];
        uint64_t cand = (uint64_t)last_val + 1;   /* search for next integer > previous */

        seq_t cand_mask;
        while (!valid_candidate(cand, forbidden, &cand_mask))
            ++cand;                   /* skip invalid candidates */

        /* Store new term. */
        ensure_capacity(seq_len + 1);
        seq_vals[seq_len]  = (seq_t)cand;
        seq_masks[seq_len] = cand_mask;
        ++seq_len;
    }

    return seq_vals[n - 1];
}
