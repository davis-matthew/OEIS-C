/*
 * A361913  a(n) is the number of steps in the main loop of the Pollard rho
 *          integer factorization algorithm for n, with x=2, y=2 and
 *          g(x)=x^2-1.
 * https://oeis.org/A361913
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of Pollard's Rho main loop with f(x)
 *            = x^2 − 1 (mod n), starting from x = y = 2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t x = 2;
    seq_t y = 2;
    idx_t steps = 0;
    while (1) {
        /* x = f(x) */
        uint64_t t = (uint64_t)x * (uint64_t)x;
        t = (t + (uint64_t)N - 1ULL) % (uint64_t)N;
        x = (seq_t)t;

        /* y = f(f(y)) */
        t = (uint64_t)y * (uint64_t)y;
        t = (t + (uint64_t)N - 1ULL) % (uint64_t)N;
        y = (seq_t)t;
        t = (uint64_t)y * (uint64_t)y;
        t = (t + (uint64_t)N - 1ULL) % (uint64_t)N;
        y = (seq_t)t;

        steps++;
        seq_t diff = x > y ? x - y : y - x;
        seq_t d = gcd(diff, N);
        if (d > (seq_t)1) {
            return (seq_t)steps;
        }
    }
}
