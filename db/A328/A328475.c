/*
 * A328475  Convert the primorial base expansion of n into its prime product
 *          form, then divide by the largest primorial which divides that
 *          product: a(n) = A111701(A276086(n)).
 * https://oeis.org/A328475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 85, u32 n <= 417, u64 n <= 2559,
 *            u128 n <= 2559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A328475:  a(n) = A111701(A276086(n))
   A276086 transforms n into primorial base digits, A111701
   converts the prime‑product form into a primorial‑reduced value.
   The implementation is O(number of primes ≈ 15) per call. */
#include <stddef.h>

static const uint32_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101
};
static const size_t PRIMES_COUNT = sizeof(PRIMES)/sizeof(PRIMES[0]);

seq_t a(idx_t n)
{
    /* 1. Primorial–base representation of n */
    idx_t remaining = n;
    idx_t product   = 1;
    idx_t digits[PRIMES_COUNT];
    for (size_t i = 0; i < PRIMES_COUNT; ++i) digits[i] = 0;

    size_t len = 0;
    while (remaining > 0 && len < PRIMES_COUNT) {
        idx_t div   = remaining / product;
        idx_t digit = div % PRIMES[len];
        digits[len] = digit;
        remaining   -= digit * product;
        product     *= PRIMES[len];
        ++len;
    }

    /* 2. Find m: longest prefix of primes with positive exponent */
    size_t m = 0;
    for (size_t k = 0; k < len; ++k) {
        if (digits[k] > 0) {
            m = k + 1;
        } else {
            break;
        }
    }

    /* 3. Compute the result: multiply primes with adjusted exponents */
    seq_t result = 1;
    for (size_t k = 0; k < len; ++k) {
        idx_t exp = digits[k];
        if (k < m) {
            exp -= 1;
        }
        for (idx_t r = 0; r < exp; ++r) {
            result *= (seq_t)PRIMES[k];
        }
    }
    return result;
}
