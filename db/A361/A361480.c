/*
 * A361480  a(n) is the greatest integer whose binary expansion has the same
 *          multiset of run-lengths as that of n.
 * https://oeis.org/A361480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is the greatest integer whose binary expansion has the same
 *            multiset of run-lengths as that of n. The algorithm builds the
 *            multiset of run lengths of n, sorts it, and then assigns the
 *            largest remaining lengths to 1‑runs and the smallest remaining
 *            to 0‑runs
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Collect the lengths of consecutive identical bits in n. */
    int runlen[64];
    int runs = 0;
    int cur_bit = 1;          /* the msb of any positive n is 1 */
    int len = 1;

    /* Find the position of the most significant bit (msb). */
    uint64_t t = n;
    int msb = 0;
    while (t >>= 1) ++msb;

    /* Scan bits from msb-1 down to 0. */
    for (int pos = msb - 1; pos >= 0; --pos) {
        int bit = (n >> pos) & 1ULL;
        if (bit == cur_bit) {
            ++len;
        } else {
            runlen[runs++] = len;
            cur_bit = bit;
            len = 1;
        }
    }
    runlen[runs++] = len;    /* store the last run */

    /* Sort the run lengths ascending (simple insertion sort). */
    for (int i = 1; i < runs; ++i) {
        int key = runlen[i];
        int j = i - 1;
        while (j >= 0 && runlen[j] > key) {
            runlen[j + 1] = runlen[j];
            --j;
        }
        runlen[j + 1] = key;
    }

    /* Assign lengths: largest to 1-runs, smallest to 0-runs. */
    int low = 0;
    int high = runs - 1;
    seq_t result = 0;

    for (int i = 0; i < runs; ++i) {
        int L = (i % 2 == 0) ? runlen[high--] : runlen[low++];
        if (i % 2 == 0) {                /* one-run */
            for (int k = 0; k < L; ++k)
                result = (result << 1) | (seq_t)1;
        } else {                         /* zero-run */
            result <<= L;
        }
    }

    return result;
}
