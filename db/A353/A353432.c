/*
 * A353432  Numbers k such that the k-th composition in standard order has its
 *          own run-lengths as a consecutive subsequence.
 * https://oeis.org/A353432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A353432.
 *
 * Definition:  a(n) is the n-th integer k (k≥0) such that the k-th
 * composition in graded reverse‑lexicographic order has its own
 * run‑lengths appearing as a contiguous subsequence of that composition.
 *
 * The algorithm tests successive k values (starting at 0), computes the
 * composition corresponding to k, determines its run‑length sequence,
 * and checks whether that run‑length sequence occurs consecutively
 * within the composition itself.  It continues until the n‑th qualifying
 * k is found.
 * No caching is used; each call recomputes from 0.  The routine is
 * O(k · L) where L is the composition length (≤ popcount(k) ≤ 64),
 * which is fast enough for n ≈ 10 000.
 */

#include <stdbool.h>
#include <stddef.h>

/* Helper: determine whether a given k satisfies the property. */
static bool qualifies(idx_t k)
{
    /* Compute the composition from k.
     * parts[i] = difference between successive positions of 1‑bits
     * in the reversed binary representation of k, with a leading 0.
     * Maximum number of parts is popcount(k) ≤ 64 for 64‑bit k. */
    idx_t parts[64];
    size_t plen = 0;
    idx_t last = 0;
    idx_t pos  = 0;
    idx_t temp = k;

    while (temp) {
        if (temp & (idx_t)1) {
            parts[plen++] = (pos + 1) - last;
            last = pos + 1;
        }
        temp >>= 1;
        pos++;
    }

    /* Empty composition (k = 0) trivially satisfies the property. */
    if (plen == 0)
        return true;

    /* Compute the run‑length sequence of the composition. */
    idx_t runs[64];
    size_t rlen = 0;
    idx_t prev = parts[0];
    size_t cnt  = 1;

    for (size_t i = 1; i < plen; i++) {
        if (parts[i] == prev) {
            cnt++;
        } else {
            runs[rlen++] = cnt;
            prev = parts[i];
            cnt = 1;
        }
    }
    runs[rlen++] = cnt;

    /* Check whether runs[] appears as a contiguous subsequence of parts[]. */
    if (rlen > plen)
        return false;

    for (size_t start = 0; start + rlen <= plen; start++) {
        bool match = true;
        for (size_t j = 0; j < rlen; j++) {
            if (parts[start + j] != runs[j]) {
                match = false;
                break;
            }
        }
        if (match)
            return true;
    }
    return false;
}

/* Public function required by the build environment. */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) = 0. */
    idx_t k     = 0;
    idx_t count = 0;

    for (;;) {
        if (qualifies(k)) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
