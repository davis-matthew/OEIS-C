/*
 * A374167  a(n) is the largest k such that the binary representation of
 *          prime(n), when interpreted as a base-b number, yields a prime for
 *          every b in 2..k.
 * https://oeis.org/A374167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 35, u32 n <= 35, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements A374167: for n ≥ 1, a(n) is the greatest
   integer k such that the binary expansion of the n‑th prime, when
   interpreted as a base‑b number, yields a prime for every b = 2..k. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* --- 1. obtain the n‑th prime ------------------------------------------ */
    uint32_t p = get_prime(n);           /* n is 1‑based according to OEIS */

    /* --- 2. determine the binary length of p ------------------------------ */
    int len = 0;
    uint64_t tmp = p;
    while (tmp) {
        len++;
        tmp >>= 1;
    }
    /* p ≥ 2, therefore len ≥ 2; no need for further checks */

    /* --- 3. search for the largest k --------------------------------------- */
    for (idx_t base = 2; ; base++) {
        seq_t value = 0;                     /* value in base `base`      */
        /* interpret the bits of p from most significant to least significant */
        for (int j = 0; j < len; j++) {
            unsigned bit = (p >> (len - 1 - j)) & 1u;
            value = value * (seq_t)base + (seq_t)bit;
        }
        if (!is_prime_big(value))            /* first non‑prime base found */
            return (seq_t)(base - 1);        /* k is the previous base       */
    }
}
