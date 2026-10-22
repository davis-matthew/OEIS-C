/*
 * A384710  a(n) = Sum_{k=0..n} [gcd(k, n) = 1], where [.] are the Iverson
 *          brackets.
 * https://oeis.org/A384710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = φ(n) + [n == 1]
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ(n) is Euler's totient function and [n == 1] is 1 if n==1, 0 otherwise. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    if (n == 1) {
        /* φ(1) = 1, plus the extra count for k = 0 when n = 1. */
        return (seq_t)2;
    }

    /* Compute Euler's totient φ(n).  The totient is always ≤ n, so it fits in seq_t. */
    seq_t result = (seq_t)n;     /* start with φ(n) = n */
    idx_t temp = n;

    /* Handle factor 2 separately. */
    if ((temp & 1) == 0) {
        result -= result / 2;
        while ((temp & 1) == 0) {
            temp >>= 1;
        }
    }

    /* Check odd divisors up to sqrt(temp).  Use division to avoid overflow. */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            result -= result / (seq_t)i;
            do {
                temp /= i;
            } while (temp % i == 0);
        }
    }

    /* If temp is now > 1, it is a prime factor larger than sqrt(original n). */
    if (temp > 1) {
        result -= result / (seq_t)temp;
    }

    return result;
}
