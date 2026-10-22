/*
 * A364767  The number of divisors of n that are practical numbers (A005153).
 * https://oeis.org/A364767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8554, u16 n <= 8554, u32 n <= 8554, u64 n <= 8554,
 *            u128 n <= 8554, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A364767, the number of divisors of n that are practical numbers. */
#include <stdbool.h>
#include <stdint.h>

/* Checks whether an integer n (>=1) is a practical number.
   Uses the known characterization:
     Let s = 1. For each prime power p^a dividing n (in increasing order of p),
       if p > s + 1 then n is not practical;
       otherwise update s = s * sum_{i=0..a} p^i.
   Returns true if n is practical, false otherwise. */
static bool is_practical(idx_t n)
{
    if (n == 1) return true;
    if (n % 2) return false;          /* all practical numbers >1 are even */

    idx_t m = n;
    idx_t p = 2;
    seq_t s = 1;                      /* s will hold sum of divisors of processed part */

    while (m > 1) {
        if (m % p == 0) {
            /* find exponent of prime p */
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* check first condition */
            if (p > (idx_t)(s + 1)) return false;

            /* compute sum of divisors of p^exp : 1 + p + p^2 + ... + p^exp */
            seq_t term = 1;
            seq_t sum  = 1;
            for (uint64_t i = 0; i < exp; ++i) {
                term *= p;
                sum  += term;
            }
            s *= sum;
        }
        if (p == 2) p = 3;
        else        p += 2;
    }
    return true;
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (is_practical(d)) ++cnt;
            idx_t e = n / d;
            if (e != d && is_practical(e)) ++cnt;
        }
    }
    return cnt;
}
