/*
 * A295294  Sum of the divisors of the powerful part of n: a(n) =
 *          A000203(A057521(n)).
 * https://oeis.org/A295294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A000203(A057521(n)) (sum of divisors of the powerful part
 *            of n)
 * Range:     u8 n <= 48, u16 n <= 8846, u32 n <= 8846, u64 n <= 8846,
 *            u128 n <= 8846, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = 1;

    /* Handle factor 2 efficiently */
    if ((n & 1) == 0) {
        idx_t e = 0;
        while ((n & 1) == 0) {
            n >>= 1;
            ++e;
        }
        if (e >= 2) {
            seq_t p   = (seq_t)2;
            seq_t pw  = 1;
            for (idx_t i = 0; i < e + 1; ++i)
                pw *= p;                           /* pw = 2^(e+1) */
            seq_t sigma = (pw - 1) / (p - 1);    /* σ(2^e) = 2^(e+1)-1 */
            result *= sigma;
        }
    }

    /* Process odd prime factors */
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) {
            idx_t e = 0;
            while (n % i == 0) {
                n /= i;
                ++e;
            }
            if (e >= 2) {
                seq_t p   = (seq_t)i;
                seq_t pw  = 1;
                for (idx_t k = 0; k < e + 1; ++k)
                    pw *= p;                       /* pw = i^(e+1) */
                seq_t sigma = (pw - 1) / (p - 1); /* σ(i^e) */
                result *= sigma;
            }
        }
    }

    /* Any remaining prime factor has exponent 1 → skip it */

    return result;
}
