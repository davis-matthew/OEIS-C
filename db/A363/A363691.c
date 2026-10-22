/*
 * A363691  Odd numbers k such that A246600(k) = 2.
 * https://oeis.org/A363691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): nth odd number k such that A246600(k)=2
   (i.e., k has exactly the two divisors 1 and k for which
   (k & d) == d). This is done by testing each odd integer
   for the divisor property.
*/
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;                    // number of valid terms seen
    for (uint64_t k = 3; ; k += 2) {    // only odd k
        bool ok = true;
        /* check all proper divisors d of k
           (both d and k/d) */
        for (uint64_t d = 3; d <= k / d; d += 2) {
            if (k % d == 0) {
                if ((k & d) == d) {   // d has no extra bits
                    ok = false;
                    break;
                }
                uint64_t d2 = k / d;  // complementary divisor
                if ((k & d2) == d2) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
