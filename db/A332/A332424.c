/*
 * A332424  If n = Product (p_j^k_j) then a(n) = Sum ((-1)^(pi(p_j) + 1) *
 *          p_j), where pi = A000720.
 * https://oeis.org/A332424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{p|n, p prime} (-1)^(pi(p)+1) * p
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   _smallest_prime_factor, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;
    while (nn > 1) {
        uint64_t p = _smallest_prime_factor(nn);
        seq_t pi = compute_pi(p);
        seq_t sign = (pi & 1) ? 1 : -1;
        sum += sign * (seq_t)p;
        while (nn % p == 0) {
            nn /= p;
        }
    }
    return sum;
}
