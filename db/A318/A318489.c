/*
 * A318489  Number of steps to reach a lower number than starting value in
 *          7x+-1 problem, or 0 if never reached.
 * https://oeis.org/A318489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of steps needed for the 7x±1 map * to produce the
 *            first value strictly less than n. * The map is: * if x is even:
 *            x -> x / 2 * if x is odd and x ≡ 1 (mod 4): x -> 7*x + 1 * if x
 *            is odd and x ≡ 3 (mod 4): x -> 7*x – 1 * (Exactly one of 7*x±1
 *            is even and divisible by 4 for every odd x...
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* required by the build system, no use of bool here */

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n == 1) return 0;

    /* Ensure the starting value is in seq_t */
    seq_t target = (seq_t)n;
    seq_t x      = target;          /* current iterate */
    seq_t count  = 0;                /* number of steps taken */

    while (1)
    {
        /* Stop as soon as we reach a value < starting value */
        if (x < target) return count;

        /* Perform one iteration according to the 7x±1 rule */
        if ((x & (seq_t)1) == 0)          /* even */
        {
            x = x >> 1;
        }
        else                              /* odd */
        {
            /* Choose +1 when x ≡ 1 (mod 4), else -1 */
            if ((x & (seq_t)3) == 1)
                x = (seq_t)7 * x + (seq_t)1;
            else
                x = (seq_t)7 * x - (seq_t)1;
        }

        ++count;
    }

    /* Unreachable, but keeps the compiler happy */
    return 0;
}
