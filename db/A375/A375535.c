/*
 * A375535  a(n) = n - A075860(n).
 * https://oeis.org/A375535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A075860(n) where A075860(n) is obtained by repeatedly
 *            replacing n by the sum of its distinct prime factors until a
 *            prime is reached (for primes the value is n itself)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Special case: n = 1: A075860(1) = 0, so a(1) = 1 */
    if (n == 1) return (seq_t)1;

    idx_t m = n;
    /* Reduce m until it becomes a prime */
    while (!is_prime(m)) {
        seq_t sum = 0;
        idx_t nn = m;

        /* Handle factor 2 separately for efficiency */
        if ((nn & 1) == 0) {
            sum += 2;
            while ((nn & 1) == 0) nn >>= 1;
        }

        /* Trial division by odd numbers up to sqrt(nn) */
        for (idx_t i = 3; i <= nn / i; i += 2) {
            if (nn % i == 0) {
                sum += i;
                while (nn % i == 0) nn /= i;
            }
        }

        /* If nn > 1, it is a prime factor > 2 */
        if (nn > 1) sum += nn;

        m = (idx_t)sum;
        /* If m becomes 0 or 1, break to avoid infinite loop */
        if (m <= 1) break;
    }

    seq_t f = (seq_t)m;          /* A075860(n) */
    return (seq_t)n - f;         /* a(n) = n - A075860(n) */
}
