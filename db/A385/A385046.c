/*
 * A385046  The sum of the unitary divisors of n that are 3-smooth numbers
 *          (A003586).
 * https://oeis.org/A385046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n, p≤3} (p^e+1) (multiplicative with a(2^e)=2^e+1,
 *            a(3^e)=3^e+1, a(p^e)=1 for p≥5)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n≥1; a(1)=1 by definition */
    seq_t result = (seq_t)1;
    uint64_t t = (uint64_t)n;

    /* exponent of 2 in n */
    int e2 = 0;
    while (t % 2ull == 0) {
        e2++;
        t /= 2ull;
    }
    if (e2 > 0) {
        seq_t p2 = (seq_t)1;
        for (int i = 0; i < e2; ++i) {
            p2 *= (seq_t)2;
        }
        result *= (p2 + (seq_t)1);
    }

    /* exponent of 3 in n */
    int e3 = 0;
    while (t % 3ull == 0) {
        e3++;
        t /= 3ull;
    }
    if (e3 > 0) {
        seq_t p3 = (seq_t)1;
        for (int i = 0; i < e3; ++i) {
            p3 *= (seq_t)3;
        }
        result *= (p3 + (seq_t)1);
    }

    return result;
}
