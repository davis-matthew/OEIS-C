/*
 * A345921  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum != 0.
 * https://oeis.org/A345921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k for which the k‑th composition in *
 *            standard order (A066099) has non‑zero alternating sum. * This
 *            implementation scans k incrementally and counts the qualifying *
 *            values
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return true iff the alternating sum of the composition associated to
 * k (A066099) is non‑zero.  The composition is obtained from the set
 * of positions of 1's in the reversed binary expansion of k. */
static bool has_nonzero_alt_sum(idx_t k)
{
    /* Gaps between consecutive set‑bit positions (1‑based). */
    idx_t gaps[64];
    int m = 0;               /* number of gaps filled */
    idx_t last_pos = 0;      /* previous 1‑bit position, 0 for none */
    idx_t bit_idx = 0;       /* current bit index (0 for LSB) */
    idx_t t = k;

    while (t) {
        if (t & 1ULL) {                /* bit is 1 */
            idx_t pos = bit_idx + 1;   /* positions are 1‑based */
            gaps[m++] = pos - last_pos;
            last_pos = pos;
        }
        t >>= 1;
        ++bit_idx;
    }

    /* Compute alternating sum over the reversed gap sequence. */
    long long sum = 0;
    int sign = ((m - 1) & 1) ? -1 : 1; /* +1 if (m-1) even, else -1 */
    for (int i = 0; i < m; ++i) {
        sum += sign * (long long)gaps[i];
        sign = -sign;
    }
    return sum != 0;
}

/* Return the n‑th term of A345921 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (has_nonzero_alt_sum(k))
            ++count;
    }
    return (seq_t)k;
}
