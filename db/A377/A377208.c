/*
 * A377208  a(n) is the number of iterations that n requires to reach a
 *          noninteger or a Fibonacci number under the map x -> x / z(x),
 *          where z(k) = A007895(k) is the number of terms in the Zeckendorf
 *          representation of k; a(n) = 0 if n is a Fibonacci number.
 * https://oeis.org/A377208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of iterations of x -> x / z(x) * where z(k) =
 *            number of terms in the Zeckendorf * representation of k. The
 *            process stops when a * non‑integer or a Fibonacci number is
 *            reached. * a(n) = 0 if n itself is a Fibonacci number
 * Range:     u8 n <= 2839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_fibonacci, zeckendorf_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_fibonacci.h"
#include "../utils/zeckendorf_terms.h"

/* Main sequence function as required. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;

    if (is_fibonacci(x))
        return (seq_t)0;

    seq_t count = (seq_t)0;
    while (true) {
        seq_t z = zeckendorf_terms(x);
        seq_t div = x / z;
        count++;

        /* Stop if the result is not an integer. */
        if (x % z != (seq_t)0)
            return count;

        /* Stop if the new integer is a Fibonacci number. */
        if (is_fibonacci(div))
            return count;

        x = div;
    }
}
