/*
 * A248797  Number of terms > 1 in Fibonacci-variation of Collatz sequence
 *          starting with (1, 2n+1).
 * https://oeis.org/A248797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms greater than 1 in the Fibonacci‑variation
 *            Collatz sequence that starts with (1, 2n+1)
 * Range:     u8 n <= 252, u16 n <= 8858, u32 n <= 8858, u64 n <= 8858,
 *            u128 n <= 8858, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t prev  = 1;
    seq_t curr  = (seq_t)(2 * n + 1);

    while (curr != 1)
    {
        count++;
        seq_t old = curr;
        curr = odd_part(prev + curr);
        prev = old;
    }

    return count;
}
