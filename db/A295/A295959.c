/*
 * A295959  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + 1, where a(0) = 1, a(1) = 5, b(0) = 2, b(1) = 3, b(2) = 4,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 5
   b(0) = 2, b(1) = 3, b(2) = 4
   a(n) = a(n-1) + a(n-2) + b(n) + 1   (n ≥ 2)
   The sequences a() and b() are increasing complementary sequences. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Persistent state between calls */
    static seq_t *a_vals = NULL;
    static seq_t *b_vals = NULL;
    static idx_t a_cap = 0, b_cap = 0;
    static idx_t computed_upto = 0;       /* largest index whose a value is known */
    static seq_t last_b = 0;              /* last computed b value (for monotone b) */
    static idx_t a_scan_ptr = 0;          /* helper for scanning a-values while finding b */

    /* ---------- initialization ---------- */
    if (!a_vals) {
        /* Allocate an initial small buffer */
        a_cap = b_cap = 8;
        a_vals = (seq_t*)malloc(a_cap * sizeof(seq_t));
        b_vals = (seq_t*)malloc(b_cap * sizeof(seq_t));
        if (!a_vals || !b_vals) abort();   /* memory error */

        /* Hard‑coded starting values */
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)5;                   /* a(1) */
        /* b(0..2) are prefilled */
        b_vals[0] = (seq_t)2;
        b_vals[1] = (seq_t)3;
        b_vals[2] = (seq_t)4;
        last_b   = b_vals[2];
        computed_upto = 1;    /* we have a(0) and a(1) ready */
    }

    /* ---------- make sure storage is large enough ---------- */
    if (n >= a_cap) {
        while (n >= a_cap) a_cap <<= 1;
        a_vals = (seq_t*)realloc(a_vals, a_cap * sizeof(seq_t));
        if (!a_vals) abort();
    }
    if (n >= b_cap) {
        while (n >= b_cap) b_cap <<= 1;
        b_vals = (seq_t*)realloc(b_vals, b_cap * sizeof(seq_t));
        if (!b_vals) abort();
    }

    /* ---------- compute values up to the requested index ---------- */
    while (computed_upto < n) {
        idx_t i = computed_upto + 1;   /* next index to fill */

        /* ---- compute b(i) for i ≥ 3 ---- */
        if (i >= 3) {
            seq_t cand = last_b + (seq_t)1;
            /* Find the smallest integer > last_b that is NOT in a(0..i-1) */
            while (1) {
                while (a_scan_ptr <= computed_upto && a_vals[a_scan_ptr] < cand)
                    ++a_scan_ptr;
                if (a_scan_ptr <= computed_upto && a_vals[a_scan_ptr] == cand)
                    ++cand;                /* cand is used by a; try next one */
                else
                    break;                   /* cand is free */
            }
            b_vals[i] = cand;
            last_b   = cand;
        }
        /* (b(i) already exists for i = 0,1,2) */

        /* ---- compute a(i) ---- */
        seq_t bi = b_vals[i];
        seq_t ai = a_vals[i-1] + a_vals[i-2] + bi + (seq_t)1;
        a_vals[i] = ai;

        ++computed_upto;
    }

    return a_vals[n];
}
