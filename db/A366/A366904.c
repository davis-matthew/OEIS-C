/*
 * A366904  The sum of exponentially evil divisors of n.
 * https://oeis.org/A366904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = 1 + Sum_{k = 1..e, k is evil} p^k
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;           /* undefined for offset 1, but safe */
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = 1;
            seq_t power = 1;
            for (int k = 1; k <= e; ++k) {
                power *= (seq_t)p;
                if (__builtin_parity(k) == 0) {   /* even number of 1-bits: evil */
                    term += power;
                }
            }
            result *= term;
        }
    }
    if (m > 1) {
        /* Remaining prime factor has exponent 1; 1 is not evil, so term = 1 */
        /* result remains unchanged */
    }
    return result;
}
