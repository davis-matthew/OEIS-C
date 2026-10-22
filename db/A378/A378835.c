/*
 * A378835  Numbers with the median of the digits equal to the digital root.
 * https://oeis.org/A378835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6825, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_matching_median
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with the median of the decimal digits equal to the digital root.
 * Computed by brute‑force testing of consecutive non‑negative integers on demand.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_matching_median.h"

/* ------- Storage for the generated sequence ------- */
static seq_t *seq_buf    = NULL;   /* array of found terms */
static idx_t seq_cap     = 0;      /* allocated capacity of seq_buf */
static idx_t seq_len     = 0;      /* how many terms are stored */
static seq_t next_candidate = 0;   /* next number to test for membership */

/* Ensure seq_buf has at least 'need' capacity. */
static void ensure_capacity(idx_t need)
{
    if (seq_cap >= need) return;
    idx_t new_cap = (seq_cap == 0) ? 16 : (seq_cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_buf = (seq_t *)realloc(seq_buf, (size_t)new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate terms until at least 'max_index' terms are stored. */
static void generate_until(idx_t max_index)
{
    while (seq_len < max_index) {
        if (has_matching_median(next_candidate)) {
            if (seq_len + 1 > seq_cap) ensure_capacity(seq_len + 1);
            seq_buf[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) corresponds to seq_buf[0]. If n == 0 the sequence is undefined. */
    if (n == 0) return 0;
    if (seq_len < n) generate_until(n);
    return seq_buf[n - 1];
}
