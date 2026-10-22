/*
 * A393595  Sequence where k is appended after every k^k occurrences of 1,
 *          with multiple values following a 1 listed in order.
 * https://oeis.org/A393595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 except that after the cumulative count of 1's reaches
 *            k^k * we append k; if the count is a common multiple of several
 *            k^k, all such k * are appended in increasing order. See OEIS
 *            A393595
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* We only need k up to a value where k^k may occur within the
     * number of 1's seen up to position n.  For n up to 10^5
     * k=12 suffices (12^12 ≈ 8.9e12).  The loop is still tiny. */
    const int K = 12;
    idx_t power[K + 1];
    idx_t thresh[K + 1];

    /* Precompute k^k for k = 2..K */
    for (int k = 2; k <= K; ++k) {
        idx_t val = 1;
        for (int i = 0; i < k; ++i)
            val *= (idx_t)k;
        power[k] = val;
        thresh[k] = val;          /* next multiple of k^k to trigger */
    }

    idx_t pos = 1;        /* 1‑based position in the sequence */
    idx_t count_ones = 0; /* number of 1's seen so far */

    while (1) {
        /* Emit a 1 at current position */
        if (pos == n)
            return (seq_t)1;              /* finished */
        pos++;             /* next position after the 1 */
        ++count_ones;

        /* Emit any k's whose threshold matches the current count_ones */
        for (int k = 2; k <= K; ++k) {
            if (thresh[k] == count_ones) {
                if (pos == n)
                    return (seq_t)k;       /* finished */
                pos++;                     /* next position after the k */
                thresh[k] += power[k];     /* next trigger for this k */
            }
        }
        /* Loop again to emit the next 1 */
    }
}
