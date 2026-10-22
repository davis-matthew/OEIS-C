/*
 * A355264  a(n) = n * largest-nth-power(n, 2) = n * A000188(n), where
 *          largest-nth-power(n, e) is the largest positive integer b such
 *          that b^e divides n.
 * https://oeis.org/A355264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * A000188(n) = n * product over prime powers p^e in n
 *            of p^{floor(e/2)} (largest square divisor of n)
 * Range:     u8 n <= 48, u16 n <= 1680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for OEIS sequence A355264 */
seq_t a(idx_t n)
{
    /* offset is 1; for n == 0 we return 0 for safety */
    if (n == 0U)
        return (seq_t)0;

    seq_t result = (seq_t)n;                /* start with n itself */
    uint64_t temp  = (uint64_t)n;            /* factorise n */

    /* Trial division using 2 and then odd numbers */
    uint64_t i = 2;
    while (i * i <= temp) {
        if (temp % i == 0) {
            int exp = 0;
            while (temp % i == 0) {
                temp /= i;
                exp++;
            }
            int half = exp >> 1;   /* floor(exp/2) */
            for (int k = 0; k < half; ++k)
                result = result * (seq_t)i;
        }
        i = (i == 2) ? 3 : i + 2;  /* move to next candidate (skip evens) */
    }

    /* If temp > 1, it is a prime > sqrt(original n); its exponent is 1
       (floor(1/2) == 0), so we do nothing. */
    return result;
}
