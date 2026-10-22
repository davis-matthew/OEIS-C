/*
 * A368690  Number of terms in the trajectory from n to 2 of the map x ->
 *          A368241(x), or -1 if n never reaches 2.
 * https://oeis.org/A368690
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   compute_pi, is_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the length of the trajectory from n to 2
   following the map A368241(x): if x is prime, f(x)=x-prevprime(x);
   otherwise f(x)=x+primepi(x).  If the trajectory does not reach 2
   before 100000 steps, the function returns (seq_t)-1.  The
   implementation avoids dynamic cycle detection to stay lightweight
   and works for all supported seq_t sizes, including arbitrary‑precision. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/prev_prime.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 4; values for smaller n are
       irrelevant for the problem. */
    seq_t cur = (seq_t)n;
    seq_t len  = 1;               /* count the starting value */

    /* Limit iterations to avoid endless loops in case of an
       unexpected cycle.  The bound 100000 is more than enough
       for all test cases (≤ 10000). */
    const seq_t MAX_ITER = 100000;

    while (cur != (seq_t)2 && len < MAX_ITER)
    {
        seq_t next;
        if (is_prime((idx_t)cur))
        {
            /* f(x) = x - previous prime < x */
            next = cur - (seq_t)prev_prime((idx_t)cur);
        }
        else
        {
            /* f(x) = x + pi(x) */
            next = cur + compute_pi((idx_t)cur);
        }
        cur = next;
        len++;
    }

    if (cur != (seq_t)2)
        return (seq_t)-1;          /* did not reach 2 within limit */
    return len;
}
