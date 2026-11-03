/*
 * A378835  Numbers with the median of the digits equal to the digital root.
 * https://oeis.org/A378835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6825, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9770 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with the median of the decimal digits equal to the digital root.
 * Computed by brute‑force testing of consecutive non‑negative integers on demand.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff x has median of its decimal digits equal to its digital root. */
static bool has_matching_median(seq_t x)
{
    /* Frequency of each decimal digit (0‑9). */
    int freq[10] = {0};
    int len = 0;           /* number of digits */
    int sum_mod9 = 0;      /* sum of digits modulo 9 */

    seq_t tmp = x;
    if (tmp == 0) {            /* special case: 0 has one digit 0 */
        freq[0] = 1;
        len = 1;
    } else {
        while (tmp != 0) {
            int d = (int)(tmp % 10);
            ++freq[d];
            ++len;
            sum_mod9 = (sum_mod9 + d) % 9;
            tmp /= 10;
        }
    }

    /* Digital root: 0 for 0, 9 for multiples of 9, otherwise sum_mod9. */
    int root = (x == 0) ? 0 : ((sum_mod9 == 0) ? 9 : sum_mod9);

    int median = -1;
    if (len & 1) {          /* odd length: single middle digit after sorting */
        int target = len / 2;
        int cum = 0;
        for (int d = 0; d <= 9; ++d) {
            cum += freq[d];
            if (cum > target) {
                median = d;
                break;
            }
        }
    } else {                /* even length: average of two middle digits */
        int pos1 = len / 2 - 1;
        int pos2 = len / 2;
        int cum = 0;
        int d1 = -1, d2 = -1;
        for (int d = 0; d <= 9; ++d) {
            int before = cum;
            cum += freq[d];
            if (d1 == -1 && before <= pos1 && cum > pos1) d1 = d;
            if (before <= pos2 && cum > pos2) { d2 = d; break; }
        }
        if (d1 == -1 || d2 == -1) return false;              /* should not happen */
        if ((d1 + d2) & 1) return false;                     /* median not integer */
        median = (d1 + d2) >> 1;                             /* integer median */
    }

    return median == root;
}

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
    seq_buf = mem_resize(seq_t, seq_buf, (size_t)new_cap);
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
