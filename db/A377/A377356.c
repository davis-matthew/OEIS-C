/*
 * A377356  a(n) = Product{i = 1..(n-1)} prime(i)^e_i, where prime(i)^e_i is
 *          the smallest power of prime(i) which exceeds prime(n).
 * https://oeis.org/A377356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{p<prime(n)} p^e_p , where e_p is the least integer
 *            with p^e_p > prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the utility functions:
   get_prime, seq_pow, seq_mul.  All functions work with the seq_t type. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* a(1) = 1 */

    /* Prime of the nth index (0‑based offset is 1). */
    uint32_t pn32 = get_prime(n);
    uint64_t prime_n = pn32;   /* use 64‑bit for comparison */

    seq_t result = (seq_t)1;

    for (idx_t k = 1; k < n; ++k) {
        uint32_t pk32 = get_prime(k);
        uint64_t pk = pk32;

        /* Find smallest exponent e such that pk^e > prime_n */
        uint64_t pow = pk;
        unsigned int e = 1;
        while (pow <= prime_n) {
            pow *= pk;
            ++e;
        }

        /* Multiply the contribution of this prime into the result. */
        seq_t power = seq_pow((seq_t)pk32, (idx_t)e);
        result = seq_mul(result, power);
    }

    return result;
}
