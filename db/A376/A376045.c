/*
 * A376045  Complement of A325112.
 * https://oeis.org/A376045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_divisible_subseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376045: the n-th positive integer whose decimal expansion contains a subsequence that is divisible by 3. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_divisible_subseq.h"

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined by OEIS, but guard anyway */

    idx_t count = 0;
    idx_t num   = 1;

    while (true) {
        if (has_divisible_subseq(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;    /* cast to the output type */
        }
        ++num;
    }
}
