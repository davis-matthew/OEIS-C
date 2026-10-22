/*
 * A396665  Smallest nonnegative integer z for which there exists a solution
 *          to n = x*y+y*z+z*x in nonnegative integers x <= y <= z.
 * https://oeis.org/A396665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest nonnegative integer z for which there exist *
 *            integers x,y with 0 ≤ x ≤ y ≤ z and n = x*y + y*z + z*x
 * Range:     u8 n <= 2756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The function works for any seq_t type (fixed‑size or arbitrary precision). */
seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;

    /* Special case: n = 0 is represented by (x,y,z) = (0,0,0). */
    if (n_val == 0)
        return 0;

    /* Iterate z in increasing order until a solution is found. */
    for (idx_t z = 1;; ++z)
    {
        seq_t z_val = (seq_t)z;

        /* For this z try all possible x with 0 ≤ x ≤ z. */
        for (idx_t x = 0; x <= z; ++x)
        {
            seq_t x_val = (seq_t)x;

            /* Compute n - z*x. If z*x > n we cannot have a solution. */
            seq_t tx = z_val * x_val;          /* z * x */
            if (n_val < tx)
                continue;

            seq_t num = n_val - tx;            /* n - z*x */
            seq_t den = z_val + x_val;         /* (x + z) > 0 for z>0 */

            if (num % den != 0)
                continue;

            seq_t y = num / den;
            if (y <= z_val && y >= x_val)
                return z_val;                  /* smallest z found */
        }
    }
}
