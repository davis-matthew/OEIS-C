/*
 * A306689  Integers m with decimal expansion m = abc...z such that m is a
 *          multiple of both az and za; z may not be zero.
 * https://oeis.org/A306689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 571, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A306689: integers m such that m is a multiple of both
   the two‑digit numbers formed by its first and last digits
   (az) and (za).  z may not be zero.  The function
   performs a brute‑force search, storing previously found
   terms in a static array so that successive calls are fast. */

/* Include helpers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t,seq_t) */

/* LCM table for 1≤a,z≤9 */
static seq_t lcm_table[10][10];
static bool lcm_table_inited = false;

/* Initialise the LCM table once */
static void init_lcm_table(void) {
    if (lcm_table_inited) return;
    for (idx_t a = 1; a <= 9; ++a) {
        seq_t az = 10 * a;   /* will add z later */
        for (idx_t z = 1; z <= 9; ++z) {
            seq_t val_az = az + z;         /* 10*a + z */
            seq_t val_za = 10 * z + a;     /* 10*z + a */
            seq_t g = gcd(val_az, val_za);
            lcm_table[a][z] = (val_az / g) * val_za;
        }
    }
    lcm_table_inited = true;
}

/* Dynamic storage for sequence values */
static seq_t *seq   = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* Last m examined (global state across calls) */
static seq_t seq_last_m = 0;

/* Grow the sequence array when needed */
static void seq_grow(void) {
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* OOM protection */
    seq = tmp;
    seq_cap = new_cap;
}

/* Append a new term to the sequence array */
static void seq_append(seq_t value) {
    if (seq_len == seq_cap) seq_grow();
    seq[seq_len++] = value;
}

/* Main function required by the test harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, harmless */

    /* Initialise LCM table on first call */
    if (!lcm_table_inited) init_lcm_table();

    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        seq_t m = seq_last_m + 1;          /* next integer to test */
        idx_t z = (idx_t)(m % 10);         /* last digit */

        if (z != 0) {
            /* Determine first digit of m */
            seq_t tmp = m;
            while (tmp >= 10) tmp /= 10;
            idx_t a_digit = (idx_t)tmp;    /* first decimal digit */

            seq_t lcm = lcm_table[a_digit][z];
            if (m % lcm == 0) seq_append(m);
        }

        seq_last_m = m;                    /* advance global state */
    }

    return seq[n - 1];
}
