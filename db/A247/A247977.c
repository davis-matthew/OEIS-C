/*
 * A247977  If n = 1 or prime, then a(n) = 0; otherwise, if n is a preprime of
 *          k-th kind, then a(n) = k.
 * https://oeis.org/A247977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 0 if n = 1 or prime; otherwise a(n) = pi(floor(sqrt(n)))
 *            – pi(lpf(n)) + 1 where pi is the prime–counting function and lpf
 *            the least prime factor. In particular a(n) = pi(sqrt(n)) -
 *            pi(lpf(n)) + 1 for composite n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, is_prime, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/prime_pi.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    if (is_prime(n))
        return 0;          /* prime numbers have value 0 */

    /* composite numbers: use the formula */
    idx_t sqrt_n = int_sqrt(n);               /* floor(sqrt(n))          */
    seq_t pi_sqrt = prime_pi(sqrt_n);         /* pi(floor(sqrt(n)))      */

    /* find least prime factor (trial division is fast for our test range) */
    idx_t lpf = 0;
    if (n % 2 == 0) {
        lpf = 2;
    } else {
        for (idx_t d = 3; d <= sqrt_n; d += 2) {
            if (n % d == 0) {
                lpf = d;
                break;
            }
        }
    }
    if (lpf == 0)          /* should not happen for composites */
        lpf = n;

    seq_t pi_lpf = prime_pi(lpf);            /* pi(lpf(n)) */
    return pi_sqrt - pi_lpf + 1;
}
