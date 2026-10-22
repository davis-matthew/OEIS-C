/*
 * A353655  Number of terms in the Fibonacci-Lucas representation of n.
 * https://oeis.org/A353655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_fib, largest_lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A353655: number of terms in the Fibonacci‑Lucas representation of n */
#include <stdbool.h>
#include "../utils/largest_fib.h"
#include "../utils/largest_lucas.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* defined only for n ≥ 1, but guard against 0 */
    idx_t rem = n;
    seq_t count = 0;
    bool fib_turn = true;        /* start with a Fibonacci term */

    while (rem > 0) {
        idx_t t;
        if (fib_turn) {
            t = largest_fib(rem);
        } else {
            t = largest_lucas(rem);
        }
        rem -= t;
        ++count;
        fib_turn = !fib_turn;
    }

    return count;
}
