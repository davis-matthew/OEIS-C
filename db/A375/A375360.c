/*
 * A375360  The maximum exponent in the prime factorization of the smallest
 *          exponentially odd number that is divisible by n.
 * https://oeis.org/A375360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max_{p^e∥n} ( e + (e % 2 == 0) ) (maximum odd exponent in
 *            the smallest exponentially odd multiple of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    idx_t temp = n;
    seq_t maxexp = 0;

    /* Trial division up to sqrt(temp) */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Make exponent odd if it is even */
            if ((e & 1) == 0)
                ++e;
            if (e > maxexp)
                maxexp = e;
        }
    }

    /* Remaining prime factor (if any) has exponent 1, already odd */
    if (temp > 1) {
        seq_t e = 1;
        if (e > maxexp)
            maxexp = e;
    }

    return maxexp;
}
