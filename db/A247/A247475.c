/*
 * A247475  Numbers formed by concatenating two successive primes and their
 *          product.
 * https://oeis.org/A247475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate( p_n, p_{n+1}, p_n * p_{n+1} ) where p_n is
 *            the n-th prime. Offset 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 25, u64 n <= 3401,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required headers for the utilities used */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k);   */
#include "../utils/concat_parts.h"  /* seq_t concat_parts(idx_t a, idx_t b); */
#include "../utils/pow10.h"        /* seq_t pow10(idx_t e);           */

seq_t a(idx_t n)
{
    /* a(1) is the first term.  For n < 1 the sequence is undefined. */
    if (n < 1) return (seq_t)0;

    /* Get the successive primes.  get_prime returns a 32‑bit value. */
    uint32_t p1 = get_prime(n);       /* p_n   */
    uint32_t p2 = get_prime(n + 1);   /* p_{n+1} */

    /* Convert to seq_t for arithmetic. */
    seq_t sp1 = (seq_t)p1;
    seq_t sp2 = (seq_t)p2;

    /* Product of the two primes. */
    seq_t prod = sp1 * sp2;

    /* Find how many decimal digits the product has. */
    int d_prod = 1;
    seq_t p10   = pow10(1);  /* 10^1 in seq_t form */
    while (prod >= p10)
    {
        ++d_prod;
        p10 = p10 * (seq_t)10;  /* shift left by one decimal place */
    }

    /* Concatenate p1 and p2 first. */
    seq_t part12 = concat_parts(p1, p2);

    /* Finally append the product: part12 * 10^d_prod + prod. */
    seq_t result = part12 * pow10(d_prod) + prod;

    return result;
}
