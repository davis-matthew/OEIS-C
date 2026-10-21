/*
 * A276415  Number of ways to write n as p + x^3 + y^4, where p is a prime,
 *          and x and y are nonnegative integers.
 * https://oeis.org/A276415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (p,x,y) | p prime, x,y≥0, p+x^3+y^4 = n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the representations of n as a prime plus a cube plus a fourth power.  */

#include <stdbool.h>

/* Simple deterministic primality test for idx_t, good enough for the
   range of primes encountered when computing a(n) for n up to at least
   10 000.  */
static bool is_prime_local(idx_t p)
{
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (idx_t d = 3; d <= p / d; d += 2)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* loop over possible y – y^4 ≤ n  */
    for (idx_t y = 0; ; ++y) {
        unsigned __int128 y2 = (unsigned __int128)y * y;
        unsigned __int128 y4 = y2 * y2;
        if (y4 > (unsigned __int128)n) break;

        idx_t rest_for_x = n - (idx_t)y4;

        /* loop over possible x – x^3 ≤ rest_for_x  */
        for (idx_t x = 0; ; ++x) {
            unsigned __int128 x3 = (unsigned __int128)x * x * x;
            if (x3 > (unsigned __int128)rest_for_x) break;

            idx_t p = rest_for_x - (idx_t)x3;
            if (is_prime_local(p)) ++count;
        }
    }

    return count;
}
