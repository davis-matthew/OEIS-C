/*
 * A347532  a(n) is the sum of the nonpowers of 2 in the 3x+1 sequence that
 *          starts at n.
 * https://oeis.org/A347532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the non powers of 2 in the 3x+1 sequence starting
 *            from n. The sequence is generated until the first power of 2 is
 *            reached
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided helper is_power_of_two(seq_t) to test
   whether a term is a power of two. */

#include <stdbool.h>
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n) {
    /* Convert the index to the sequence type. */
    seq_t cur = (seq_t)n;
    seq_t sum = 0;

    /* Edge case for n == 0 (outside OEIS offset); return 0. */
    if (cur == 0) return 0;

    /* Generate the 3x+1 sequence until a power of 2 is reached. */
    while (!is_power_of_two(cur)) {
        /* Accumulate the current term, which is not a power of 2. */
        sum += cur;

        /* Compute the next term in the Collatz sequence. */
        if ((cur & 1) == 1)          /* odd */
            cur = 3 * cur + 1;
        else                         /* even */
            cur >>= 1;                /* divide by 2 */
    }

    return sum;
}
