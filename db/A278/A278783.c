/*
 * A278783  Numbers m such that A000041(m) is of the form 2^6 * k for odd k.
 * https://oeis.org/A278783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m such that the partition number p(m) has exactly * 6
 *            factors of 2, i.e. v2(p(m)) = 6
 * Range:     u8 n <= 2, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The function builds the sequence incrementally.  It stores all
 * computed partition values modulo 128 in a dynamically sized array.
 * It also collects the indices m for which the value is congruent
 * to 64 (mod 128), i.e. where v2(p(m)) = 6.  The results are kept
 * between calls so that the function works correctly for any order
 * of requests. */
seq_t a(idx_t n)
{
    /* Storage for the indices m that satisfy the condition.  We keep
       them in an array of idx_t so that very large m values can be
       stored before they are cast to seq_t when returned. */
    static idx_t *matches = NULL;
    static size_t matches_len = 0, matches_cap = 0;

    /* Storage for partition numbers modulo 128; p(0) = 1 is the first
       entry. */
    static unsigned char *rem = NULL;
    static size_t rem_len = 0, rem_cap = 0;

    /* First‑time initialization */
    if (rem_cap == 0) {
        rem_cap = 16;
        rem = (unsigned char *)malloc(rem_cap);
        rem[0] = 1;                /* p(0) = 1 */
        rem_len = 1;
    }
    if (matches_cap == 0) {
        matches_cap = 16;
        matches = (idx_t *)malloc(matches_cap * sizeof(idx_t));
        matches_len = 0;
    }

    /* Grow the sequence until we have produced n terms. */
    while (matches_len < (size_t)n) {
        /* Compute p(rem_len) modulo 128 using Euler's recurrence. */
        idx_t i = rem_len;  /* index of the new partition number */
        int sum = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)((k * (3 * k - 1)) / 2);
            if (pent1 > i) break;
            int sign = (k & 1) ? 1 : -1;
            sum += sign * (int)rem[i - pent1];
            idx_t pent2 = (idx_t)((k * (3 * k + 1)) / 2);
            if (pent2 <= i)
                sum += sign * (int)rem[i - pent2];
        }
        int val = sum % 128;
        if (val < 0) val += 128;      /* ensure non‑negative modulo */

        /* Store the new value. */
        rem[rem_len++] = (unsigned char)val;

        /* If the new value is 64 (mod 128), its 2‑adic valuation is 6. */
        if (val == 64) {
            if (matches_len == matches_cap) {
                matches_cap *= 2;
                matches = (idx_t *)realloc(matches, matches_cap * sizeof(idx_t));
            }
            matches[matches_len++] = i;
        }

        /* Resize the remainder array if necessary. */
        if (rem_len == rem_cap) {
            rem_cap *= 2;
            rem = (unsigned char *)realloc(rem, rem_cap);
        }
    }

    /* Return the desired term cast to seq_t. */
    return (seq_t)matches[(size_t)n - 1];
}
