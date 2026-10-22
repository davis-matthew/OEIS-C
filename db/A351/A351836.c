/*
 * A351836  Smallest evil number k (member of A001969) such that k*n is also
 *          evil.
 * https://oeis.org/A351836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_evil_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function finds the smallest odd evil number k such that k*(n) is
 * also evil.  An evil number has an even number of 1-bits in its binary
 * representation.  The routine tests successive odd integers, starting
 * with 3, and checks the evilness of k and k*n.  It loops until both
 * conditions hold and then returns k. */

#include <stdbool.h>
#include "../utils/is_evil_seq.h"

seq_t a(idx_t n)
{
    /* The first positive odd evil number is 3.  All solutions are odd,
     * so we can safely skip even candidates by adding 2 each step. */
    const seq_t two = (seq_t)2;
    seq_t k = (seq_t)3;    /* first odd candidate */

    while (true) {
        /* Check if k itself is evil and whether k*n is also evil. */
        if (is_evil_seq(k)) {
            seq_t prod = k * (seq_t)n;
            if (is_evil_seq(prod))
                return k;
        }
        k += two;   /* next odd integer */
    }
}
