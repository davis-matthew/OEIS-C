/*
 * A379863  a(n) is the number of steps in a Pollard rho like integer
 *          factorization algorithm for m = 2*n + 1 with f(x) = 2^x mod m and
 *          starting from x = y = 1.
 * https://oeis.org/A379863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by simulating a Pollard‑rho style factorisation
 * with m = 2*n + 1 and f(x) = 2^x mod m, starting from x = y = 1.
 * It returns the number of iterations until gcd(x−y, m) ≠ 1. */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0; return 0 for safety */

    /* m = 2*n + 1  (always odd) */
    seq_t m = (seq_t)n * 2 + 1;

    seq_t x = 1;
    seq_t y = 1;
    seq_t steps = 0;

    while (true) {
        /* advance the “tortoise” */
        x = mod_pow(2, (uint64_t)x, m);

        /* advance the “hare” two steps at a time */
        seq_t fy = mod_pow(2, (uint64_t)y, m);
        y   = mod_pow(2, (uint64_t)fy, m);

        ++steps;

        /* compute |x−y| */
        seq_t diff = (x > y) ? (x - y) : (y - x);

        /* stop when a non‑trivial gcd is found */
        if (gcd(diff, m) != 1)
            return steps;
    }
}
