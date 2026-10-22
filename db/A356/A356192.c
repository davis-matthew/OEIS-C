/*
 * A356192  a(n) is the smallest cubefull exponentially odd number (A335988)
 *          that is divisible by n.
 * https://oeis.org/A356192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   For a prime power p^e, let k = max(e,3) if e is odd, = e+1 if e
 *            is even. a(p^e) = p^k. Since a is multiplicative, a(n)=∏
 *            a(p_i^e_i) over all prime powers of n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;
    }

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            int k;
            if (e & 1) {               /* e is odd */
                k = (e > 3) ? e : 3;   /* max(e,3) */
            } else {                   /* e is even */
                k = e + 1;             /* make exponent odd */
            }
            seq_t term = 1;
            for (int i = 0; i < k; ++i) {
                term *= (seq_t)p;
            }
            result *= term;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        uint64_t p = temp;
        int e = 1;                       /* exponent is 1 */
        int k = 3;                       /* since e is odd and <3 */
        seq_t term = 1;
        for (int i = 0; i < k; ++i) {
            term *= (seq_t)p;
        }
        result *= term;
    }

    return result;
}
