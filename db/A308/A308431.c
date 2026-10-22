/*
 * A308431  For any Gaussian integer z, let f(z) = z / (1+i) when z is a
 *          multiple of 1+i and f(z) = z+1 otherwise (where i denotes the
 *          imaginary unit). a(n) gives the number of iterations of the map f,
 *          starting with n, needed to reach a cycle.
 * https://oeis.org/A308431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(z) = z/(1+i) if (Re(z)+Im(z))
 *            even, z+1 otherwise until z is in the cycle {0, -i, -i+1}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   next_gaussian
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The map acts on Gaussian integers.  Start with z = n + 0·i. */

#include <stdint.h>
#include "../utils/next_gaussian.h"

seq_t a(idx_t n)
{
    int64_t u = (int64_t)n;   /* starting real part */
    int64_t v = 0;           /* starting imaginary part */
    seq_t steps = 0;

    /* Stop when reaching any element of the cycle {0, -i, -i+1}. */
    while (!((u == 0 && v == 0)   /* 0  */
          || (u == 0 && v == -1) /* -i */
          || (u == 1 && v == -1))) { /* -i+1 */

        next_gaussian(&u, &v);
        ++steps;
    }

    return steps;
}
