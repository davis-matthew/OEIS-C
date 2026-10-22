/*
 * A384883  Number of maximal sparse subsets of the binary indices of n, where
 *          a set is sparse iff 1 is not a first difference.
 * https://oeis.org/A384883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over runs of consecutive 1‑bits in n of
 *            M(run_length), where M(0)=1, M(1)=1, M(2)=2, M(3)=2 and
 *            M(k)=M(k-2)+M(k-3) for k≥4. This counts maximal sparse subsets
 *            (independent sets with no adjacency) of the binary indices of n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* precompute M(k) for k up to 64 (max bit length of idx_t) */
    static seq_t M[65];
    static bool initialized = false;
    if (!initialized) {
        M[0] = 1;
        M[1] = 1;
        M[2] = 2;
        M[3] = 2;
        for (idx_t i = 4; i < 65; ++i) {
            M[i] = M[i-2] + M[i-3];
        }
        initialized = true;
    }

    seq_t result = 1;
    idx_t tmp = n;
    bool in_run = false;
    idx_t run_len = 0;

    /* scan bits of n, counting consecutive 1‑runs */
    while (tmp) {
        if (tmp & 1ULL) {
            if (!in_run) {          /* start a new run */
                in_run = true;
                run_len = 1;
            } else {
                ++run_len;           /* extend current run */
            }
        } else {
            if (in_run) {           /* end of a run */
                if (run_len >= 2)
                    result = result * M[run_len];
                in_run = false;
            }
        }
        tmp >>= 1;
    }

    /* handle run that may end exactly at the most‑significant bit */
    if (in_run && run_len >= 2) {
        result = result * M[run_len];
    }

    return result;
}
