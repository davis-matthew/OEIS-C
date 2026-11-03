/*
 * A332824  a(n) = Product_{d|n} A019565(phi(d)), where phi is Euler totient
 *          function A000010.
 * https://oeis.org/A332824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A019565(phi(d))
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 71, u64 n <= 251,
 *            u128 n <= 791, big any n
 * Validated: standard, pass, 71 terms through n = 7742 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Must be compiled for any integer type seq_t/idx_t as specified in the build. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/totient_seq.h"

/* Helper: compute A019565(n) – product of primes whose 1-bits
   in the binary representation of n indicate the corresponding
   prime position: bit 0 -> 2, bit 1 -> 3, bit 2 -> 5, etc. */
static seq_t A019565_value(idx_t m)
{
    static const uint32_t primes[64] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
        47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103,
        107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163,
        167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227,
        229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311
    };

    seq_t result = (seq_t)1;
    idx_t bit = 0;
    while (m) {
        if (m & 1U) {
            result *= (seq_t)primes[bit];
        }
        m >>= 1;
        ++bit;
    }
    return result;
}

/* Main function computing the sequence A332824. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset – harmless */

    seq_t prod = (seq_t)1;

    for (idx_t d = 1; d*d <= n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;

            /* process divisor d */
            idx_t phi_d = (idx_t)totient_seq((seq_t)d);
            prod *= A019565_value(phi_d);

            /* process complementary divisor other if distinct */
            if (other != d) {
                idx_t phi_o = (idx_t)totient_seq((seq_t)other);
                prod *= A019565_value(phi_o);
            }
        }
    }

    return prod;
}
