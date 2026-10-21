/*
 * A265350  Numbers whose factorial base representation (A007623) contains at
 *          least one of the nonzero digits occurs more than once (although
 *          not necessarily in adjacent positions).
 * https://oeis.org/A265350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   ensure_cap_seq, has_repeated_nonzero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A265350 by enumerating natural numbers
   and testing whether the factorial base representation contains any
   non‑zero digit twice.  The list is generated lazily and cached so that
   each integer is examined at most once, regardless of call order.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ensure_cap_seq.h"
#include "../utils/has_repeated_nonzero_digit.h"

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Static state to cache generated sequence values. */
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t candidate = 2;      /* smallest number to test (1 and 2 already skipped) */

    if (n == 0) /* undefined, but guard anyway */
        return (seq_t)0;

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        if (has_repeated_nonzero_digit((seq_t)candidate)) {
            ensure_cap_seq(seq_len + 1, &seq_arr, &seq_cap);
            seq_arr[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
    return seq_arr[n - 1];
}
