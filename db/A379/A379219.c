/*
 * A379219  Dirichlet inverse of A379218.
 * https://oeis.org/A379219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Π_{p^e‖n} f(p^e) * f(2^e) = -1 * if p is a Mersenne
 *            prime: * f(p) = 1 - p * f(p^2) = -p * f(p^e) = 0 (e ≥ 3) *
 *            otherwise: * f(p) = -p * f(p^e) = 0 (e ≥ 2)
 * Range:     i8 n <= 130, i16 n <= 9443, i32 n <= 9443, i64 n <= 9443,
 *            i128 n <= 9443, big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        seq_t factor;
        if (p == 2) {
            /* a(2^e) = -1 for all e ≥ 1 */
            factor = -1;
        } else {
            /* check if p is a Mersenne prime */
            bool is_mersenne = false;
            uint64_t x = p + 1;
            if ((x & (x - 1)) == 0) {              /* x is a power of two   */
                unsigned int q = 0;
                while (x > 1) { x >>= 1; q++; }   /* log2(p+1) */
                if (is_prime(q)) {
                    is_mersenne = true;
                }
            }

            if (is_mersenne) {
                if (exp == 1) {
                    factor = (seq_t)1 - (seq_t)p;
                } else if (exp == 2) {
                    factor = -(seq_t)p;
                } else {
                    factor = 0;
                }
            } else {
                if (exp == 1) {
                    factor = -(seq_t)p;
                } else {
                    factor = 0;
                }
            }
        }

        if (factor == 0) {
            return 0;
        }

        result *= factor;
    }

    return result;
}
