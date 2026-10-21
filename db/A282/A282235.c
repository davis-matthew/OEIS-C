/*
 * A282235  a(n) = Fibonacci(n) represented in bijective base-4 numeration.
 * https://oeis.org/A282235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = bijective base‑4 representation of Fibonacci(n)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   _bij4_from_seq, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the provided fast Fibonacci routine. */
#include "../utils/fib.h"
#include "../utils/_bij4_from_seq.h"

/* Public sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)          /* behaviour below the offset is irrelevant */
        return 0;
    seq_t f = fib(n);           /* Fibonacci(n) */
    return _bij4_from_seq(f);
}
