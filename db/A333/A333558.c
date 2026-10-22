/*
 * A333558  a(n) = Sum_{d|n} phi(d) * prime(d).
 * https://oeis.org/A333558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * prime(d)
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t phi1 = phi(d1);
            seq_t prime1 = (seq_t)get_prime(d1);
            sum += phi1 * prime1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t phi2 = phi(d2);
                seq_t prime2 = (seq_t)get_prime(d2);
                sum += phi2 * prime2;
            }
        }
    }
    return sum;
}
