/*
 * A340407  a(n) gives the number of side-branches that will be passed, if
 *          A342369 is used to trace the Collatz tree backward starting at
 *          6*n-2 with n > 0.
 * https://oeis.org/A340407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of side‑branches encountered while tracing the
 *            Collatz tree backward from 6*n-2 using the function A342369. The
 *            result is obtained by repeatedly applying the rule: if k % 3 ==
 *            2 -> k = (2*k - 1) / 3 (odd predecessor) else -> k = 2*k (even
 *            predecessor) The count increases for every s...
 * Range:     u8 n <= 13, u16 n <= 4999, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   next_a342369
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_a342369.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)   /* out of range, return 0 for safety */
        return 0;

    seq_t count = 0;
    seq_t k = next_a342369(6 * (seq_t)n - 2); /* start at A342369(6*n-2) */

    while (k % 3 != 0) {                    /* stop when divisible by 3 */
        if (k % 3 == 2)
            count++;                        /* side‑branch occurs */
        k = next_a342369(k);
    }

    return count;
}
