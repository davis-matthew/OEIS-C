/*
 * A258022  Nonnegative integers n with property that when starting from x=n,
 *          the map x -> floor(tan(x)) reaches [the fixed point] 0 (or any
 *          other integer less than 1 if such negative fixed points exist).
 * https://oeis.org/A258022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A258022: integers n such that repeated application of
   f(x) = floor(tan(x)) eventually reaches the fixed point 0.
   Equivalently, n is NOT an integer with floor(tan(n)) = 1,
   because 1 is a fixed point of f. This function returns the
   n‑th term (offset 1) of that sequence.
*/
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 0; ; ++m)
    {
        long double t = tanl((long double)m);
        long double fl = floorl(t);

        /* exclude numbers that map directly to the fixed point 1 */
        if (fl != 1.0L)
        {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
