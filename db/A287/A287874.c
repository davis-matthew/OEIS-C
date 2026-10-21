/*
 * A287874  Concatenate prime factorization as in A080670, but write
 *          everything in binary.
 * https://oeis.org/A287874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the binary representations of the prime
 *            factorization of n (primes and exponents, exponents >1 only),
 *            interpreting the resulting string of 0/1 digits as a decimal
 *            number
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 179, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

/* bit-length of a positive uint64_t */
static uint64_t bitlen_uint64(uint64_t v)
{
    uint64_t len = 0;
    while (v) { v >>= 1; ++len; }
    return len;
}

/* Append a binary part (prime or exponent) to the running result.
   The part is treated as a string of 0/1 digits, and the result
   is the decimal number obtained by concatenating these digits. */
static inline seq_t add_part(seq_t cur, uint64_t val)
{
    uint64_t bits = bitlen_uint64(val);
    seq_t part = 0;

    for (uint64_t i = 0; i < bits; ++i) {
        uint64_t bit = (val >> (bits - 1 - i)) & 1ULL;
        part = part * 10 + bit;
    }

    seq_t powb = pow10(bits);
    return cur * powb + part;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* convention */

    uint64_t m = (uint64_t)n;
    seq_t result = 0;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++exp; }

        result = add_part(result, 2ULL);
        if (exp > 1) result = add_part(result, exp);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) { m /= p; ++exp; }

            result = add_part(result, p);
            if (exp > 1) result = add_part(result, exp);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        result = add_part(result, m);   /* exponent is 1 */
    }

    return result;
}
