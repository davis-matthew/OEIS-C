/*
 * A353167  Polynomials over GF(2) that are divisible by (x+1)^2 = x^2+1,
 *          encoded as binary numbers.
 * https://oeis.org/A353167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   has_even_parity_at_even_and_odd_positions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353167: numbers whose binary representation has an even
 * number of 1‑bits at even positions and an even number of 1‑bits at odd
 * positions.  Implementation searches through integers sequentially,
 * counting those that satisfy the parity condition. */

#include <stdbool.h>
#include "../utils/has_even_parity_at_even_and_odd_positions.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1 – not used, but kept for completeness */
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t cand = 0; ; ++cand) {
        if (has_even_parity_at_even_and_odd_positions(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
