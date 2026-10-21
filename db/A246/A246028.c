/*
 * A246028  a(n) = Product_{i in row n of A245562} Fibonacci(i+1).
 * https://oeis.org/A246028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of Fibonacci(length+1) over each run of 1's in
 *            the binary representation of n
 * Range:     u8 n <= 8190, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* no runs of 1's → product over empty set */

    seq_t prod = (seq_t)1;
    bool in_run = false;
    idx_t run_len = 0;
    idx_t temp = n;

    while (temp) {
        if (temp & 1) {                    /* bit is 1 */
            if (!in_run) {
                in_run = true;
                run_len = 1;
            } else {
                run_len++;
            }
        } else {                           /* bit is 0, end of a run if any */
            if (in_run) {
                prod *= fib(run_len + 1);   /* Fibonacci(run_len+1) */
                in_run = false;
            }
        }
        temp >>= 1;                         /* move to next lower bit */
    }

    /* If the number ends with a run of 1's, multiply it out */
    if (in_run)
        prod *= fib(run_len + 1);

    return prod;
}
