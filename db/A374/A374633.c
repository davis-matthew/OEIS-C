/*
 * A374633  Numbers k such that the leaders of weakly increasing runs in the
 *          k-th composition in standard order (A066099) are identical.
 * https://oeis.org/A374633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer k such that the leaders of weakly increasing
 *            runs in the k-th composition in standard order (OEIS A066099)
 *            are all equal. Computation is done by a linear search over k (no
 *            closed form)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_good_k(idx_t k)
{
    /* Build positions of 1 bits in the reversed binary expansion of k.
       Positions are 1‑indexed, with 0 pre‑pended. */
    int pos[65];
    int poscnt = 0;
    pos[0] = 0;
    int bit = 1;
    idx_t tmp = k;
    while (tmp) {
        if (tmp & (idx_t)1) {
            pos[++poscnt] = bit;
        }
        bit++;
        tmp >>= 1;
    }

    /* diff_count = number of parts in the composition */
    int diff_count = poscnt;
    if (diff_count == 0)   /* empty composition (k = 0) */
        return true;

    /* Build the parts of the composition in correct order. */
    int parts[65];
    for (int i = 0; i < diff_count; ++i) {
        int diff = pos[i + 1] - pos[i];
        parts[diff_count - 1 - i] = diff;   /* reverse order */
    }

    /* Check that all leaders of weakly increasing runs are identical. */
    int expected = parts[0];
    for (int i = 1; i < diff_count; ++i) {
        if (parts[i] < parts[i - 1]) {       /* start of new run */
            if (parts[i] != expected)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        if (is_good_k(k)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
