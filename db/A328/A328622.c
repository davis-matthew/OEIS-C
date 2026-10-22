/*
 * A328622  In primorial base representation of n, multiply by 2 all other
 *          digits except the least significant, and reduce each such product
 *          modulo prime(k) (to get the new digit), where k > 1 is the
 *          position of the digit, then convert back to decimal.
 * https://oeis.org/A328622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276085(A328618(A276086(n)))
 * Range:     u8 n <= 209, u16 n <= 9791, u32 n <= 9791, u64 n <= 9791,
 *            u128 n <= 9791, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Primorial‑base representation: double all digits except the least */
/* significant digit and reduce them modulo the corresponding prime, */
/* then recombine into a decimal value.) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n = 0 return 0 immediately. */
    if (n == 0)
        return 0;

    /* --- 1. Convert n to primorial base (digits < corresponding prime). -- */
    size_t cap = 8;                     /* initial capacity for digits array    */
    uint32_t *digits = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!digits)                        /* out‑of‑memory → fallback 0          */
        return 0;

    size_t len = 0;
    seq_t cur = (seq_t)n;               /* use seq_t to avoid overflow          */

    while (cur > 0) {
        if (len == cap) {
            cap <<= 1;
            uint32_t *tmp = (uint32_t *)realloc(digits, cap * sizeof(uint32_t));
            if (!tmp) {
                free(digits);
                return 0;
            }
            digits = tmp;
        }
        /* prime for this digit position (1‑based). */
        uint64_t p = get_prime((idx_t)(len + 1));
        digits[len] = (uint32_t)(cur % p);
        cur /= p;
        ++len;
    }

    /* --- 2. Double all digits except the least significant one
           and reduce modulo the corresponding prime.                      */
    for (size_t i = 1; i < len; ++i) {   /* i==1 corresponds to position 2  */
        uint64_t p = get_prime((idx_t)(i + 1));
        digits[i] = (uint32_t)(((uint64_t)2 * digits[i]) % p);
    }

    /* --- 3. Reconstruct the result from the modified digits.           */
    seq_t res = 0;
    seq_t weight = 1;
    for (size_t i = 0; i < len; ++i) {
        uint64_t p = get_prime((idx_t)(i + 1));
        res += (seq_t)digits[i] * weight;
        weight *= (seq_t)p;
    }

    free(digits);
    return res;
}
