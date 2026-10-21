/*
 * A277008  Numbers k such that in the binary expansion of k no run of 1-bits
 *          is longer than 1 + the total number of 0-bits anywhere to the
 *          right of that run.
 * https://oeis.org/A277008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A277008: numbers k such that in the binary expansion
 * of k no run of 1-bits is longer than 1 + the total number of 0‑bits
 * to the right of that run.  The function scans integers from 0 upward
 * and counts those that satisfy the condition until the n‑th one is
 * found.  The algorithm is O(a(n)). */
#include <stdbool.h>

/* Test whether a given number satisfies the run‑length condition. */
static bool is_valid(seq_t k)
{
    seq_t zeros   = 0;   /* zeros seen to the right of the current run   */
    seq_t run_len = 0;   /* length of the current run of 1‑bits          */
    while (k != 0) {
        bool bit = (k & 1) != 0;
        if (bit) {
            ++run_len;                   /* extend a run of 1‑bits            */
        } else {
            if (run_len != 0) {           /* finish a run                      */
                if (run_len > zeros + 1)
                    return false;
                run_len = 0;
            }
            ++zeros;                       /* count a zero to the right         */
        }
        k >>= 1;                           /* move to higher bit                */
    }
    if (run_len != 0)                     /* final run in the most significant bit */
        return run_len <= zeros + 1;
    return true;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of valid values seen so far    */
    seq_t k = 0;              /* candidate number                      */

    while (true) {
        if (is_valid(k)) {
            if (count == n)
                return k;
            ++count;
        }
        ++k;
    }
}
