/*
 * A383967  Inventory sequence recording number of terms with 1,2,3,...
 *          decimal digits. Count until occurrence of a term = 0, whereupon
 *          reset the count; continue.
 * https://oeis.org/A383967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 615, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A383967 – Inventory sequence recording the number of
 * terms with 1,2,3,… decimal digits.
 *   a(1)=0
 *   For each term a(n) (n>1) set d to the current target digit length,
 *   a(n)=count of already generated terms whose decimal length equals d.
 *   Append a(n) to the sequence; increment the counter for its length.
 *   If a(n)=0 reset d to 1, otherwise d++.
 *   Continue until the requested index n is reached.  The sequence
 *   is memoised across calls, so any order of queries is supported.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_len.h"

#define MAX_DIGITS 128            /* enough for seq_t up to 128 bits */

static seq_t *seq   = NULL;          /* array of computed terms   */
static idx_t seq_len = 0;            /* number of terms stored    */
static idx_t seq_cap = 0;            /* allocated capacity        */

static seq_t counts[MAX_DIGITS] = {0}; /* counts of terms by digit len */
static unsigned char cur_d = 1;        /* current target digit length */

/* Ensure the sequence buffer can hold at least `min_cap` elements. */
static void ensure_seq_cap(idx_t min_cap)
{
    if (seq_cap >= min_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Append a new term to the sequence and update counters. */
static void append_term(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq[seq_len++] = val;

    int d = digit_len(val);            /* number of decimal digits of val */
    if (d < 0) d = 0;                   /* safety, should not occur */
    if (d >= MAX_DIGITS) d = MAX_DIGITS - 1; /* clamp to array bounds */
    counts[d] += (seq_t)1;              /* increment counter for this length */
}

/* Public function: compute a(n) with 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined by OEIS, return 0 */

    /* Initialise on first use. */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = (seq_t *)malloc((size_t)seq_cap * sizeof(seq_t));
        seq_len = 0;

        /* reset counts array */
        for (int i = 0; i < MAX_DIGITS; ++i) counts[i] = (seq_t)0;
        cur_d = 1;

        /* a(1) is defined to be 0 */
        append_term((seq_t)0);
    }

    /* Generate terms up to n if necessary. */
    while (seq_len < n) {
        seq_t next = (cur_d < MAX_DIGITS) ? counts[cur_d] : (seq_t)0;
        append_term(next);

        if (next == (seq_t)0) {
            cur_d = 1;                       /* reset after a zero term */
        } else {
            cur_d++;                          /* go to the next digit length */
        }
    }

    return seq[n - 1];
}
