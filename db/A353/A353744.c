/*
 * A353744  Numbers k such that the k-th composition in standard order has all
 *          equal run-lengths.
 * https://oeis.org/A353744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353744: a(n) is the n-th nonnegative integer k such that the
   k-th composition in standard order has all equal run‑lengths.
   Implementation enumerates k progressively. */
#include <stdbool.h>
#include <stddef.h>

/* Test whether the k‑th composition (in standard order) has all equal run‑lengths. */
static bool equal_run_lengths(idx_t k)
{
    idx_t tmp = k;
    idx_t diffs[64];            /* at most 64 set bits in a 64‑bit word */
    size_t t = 0;
    idx_t prev = 0;
    idx_t bit = 1;              /* positions are 1‑based */

    /* Build the list of differences between consecutive positions of set bits. */
    while (tmp) {
        if (tmp & 1ULL) {
            diffs[t++] = bit - prev;
            prev = bit;
        }
        tmp >>= 1;
        ++bit;
    }

    /* Empty composition or single part always satisfies the condition. */
    if (t <= 1) return true;

    /* Scan the parts in reverse order (reverse of the differences). */
    idx_t last = diffs[t - 1];
    unsigned run_len = 1;       /* current run length */
    unsigned target = 0;        /* run length that all runs must match */
    bool first = true;          /* flag for first run encountered */

    for (size_t i = t - 2; ; --i) {
        idx_t part = diffs[i];
        if (part == last) {
            ++run_len;
        } else {
            if (first) {
                target = run_len;
                first = false;
            } else if (run_len != target) {
                return false;
            }
            last = part;
            run_len = 1;
        }
        if (i == 0) {                /* finished processing the last part */
            if (first) {             /* only one run exists */
                target = run_len;
            } else if (run_len != target) {
                return false;
            }
            break;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        if (equal_run_lengths(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}
