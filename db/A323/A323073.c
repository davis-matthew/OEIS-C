/*
 * A323073  Number of iterations of A049820(x) = x - A000005(x) needed to
 *          reach either zero or such x that x and A049820(x) are coprime,
 *          when starting from x = n.
 * https://oeis.org/A323073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 943, u16 n <= 9941, u32 n <= 9941, u64 n <= 9941,
 *            u128 n <= 9941, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A323073
 * Formula: a(n) = number of iterations of the map x -> x - τ(x)
 *           until x==0 or gcd(x, x-τ(x))==1, with a(0)=0.
 *   τ(x) is the number of divisors of x (A000005).
 *
 * This implementation iteratively applies the transformation until
 * the stopping condition is met, using a simple trial‑division
 * routine to compute τ(x).  The gcd is obtained from the shared
 * utilities (../utils/gcd.h).  The function works for any integer
 * index type supplied by the build (idx_t) and returns the result
 * as the designated `seq_t` type.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* Return number of iterations required for n */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t x = n;          /* current value of the process */
    seq_t cnt = 0;        /* number of performed iterations */

    while (x > 0)
    {
        /* Compute τ(x) via trial division (x fits in uint64_t). */
        uint64_t m = (uint64_t)x;
        uint64_t tau = 1;
        for (uint64_t p = 2; p * p <= m; ++p)
        {
            if (m % p == 0)
            {
                uint64_t exp = 0;
                while (m % p == 0)
                {
                    m /= p;
                    ++exp;
                }
                tau *= (exp + 1);
            }
        }
        if (m > 1) /* a remaining prime factor */
            tau *= 2;

        /* Next value after the iteration */
        seq_t y = (seq_t)x - (seq_t)tau;

        /* If stopping condition met, break.  No further count. */
        if (gcd((seq_t)x, y) == (seq_t)1)
            break;

        /* Continue loop with updated value and increment count. */
        x = (idx_t)y;
        ++cnt;
    }

    return cnt;
}
