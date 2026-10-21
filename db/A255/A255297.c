/*
 * A255297  Number of ON cells after n generations of the odd-rule cellular
 *          automaton defined by OddRule 035 when started with a single ON
 *          cell.
 * https://oeis.org/A255297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over runs of consecutive 1's in the binary *
 *            representation of n of (2*3^L - 2^L), with a(0) = 1. * This
 *            implements the run‑length transform of the sequence * A027649
 *            where f(L) = 2*3^L - 2^L
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    bool in_run = false;
    idx_t run_len = 0;

    idx_t bits = n;
    while (bits != 0) {
        if (bits & (idx_t)1) {          /* bit is 1 */
            if (!in_run) {              /* start of a new run */
                run_len = 1;
                in_run = true;
            } else {                    /* continuation of a run */
                run_len++;
            }
        } else {                        /* bit is 0 */
            if (in_run) {               /* end of a run */
                /* compute f(run_len) = 2*3^run_len - 2^run_len */
                seq_t p3 = big_pow((seq_t)3, run_len);
                seq_t p2 = big_pow((seq_t)2, run_len);
                seq_t fval = ((seq_t)2 * p3) - p2;
                result *= fval;
                in_run = false;
            }
        }
        bits >>= 1;
    }

    /* process a trailing run (if the number ends with 1) */
    if (in_run) {
        seq_t p3 = big_pow((seq_t)3, run_len);
        seq_t p2 = big_pow((seq_t)2, run_len);
        seq_t fval = ((seq_t)2 * p3) - p2;
        result *= fval;
    }

    return result;
}
