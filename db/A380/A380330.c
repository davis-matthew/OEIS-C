/*
 * A380330  a(n) = A151800(prime(n)*A151800(n)).
 * https://oeis.org/A380330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A151800(prime(n) * A151800(n))
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is implemented by using the utilities
   get_prime(idx_t)  – returns the nth prime (1‑based)
   is_prime_big(seq_t) – tests primality of an arbitrary‑precision integer */

/* Two utilities are needed from the repository */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* Compute the smallest prime strictly greater than x
   (i.e. A151800(x)).  Works for any seq_t that supports the
   ordinary arithmetic operators defined in the requirements. */
static seq_t next_prime_seq(seq_t x)
{
    /* By definition A151800(1) = 2 */
    if (x < 2)
        return 2;

    seq_t cand = x + 1;
    /* Skip even numbers, leaving cand odd (unless it is 2) */
    if ((cand & 1) == 0)
        cand += 1;

    for (;; cand += 2)          /* increasing only odd candidates */
    {
        if (is_prime_big(cand))
            return cand;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* prime(n) – the nth prime (1‑based) */
    seq_t prime_n = (seq_t)get_prime(n);

    /* A151800(n) – smallest prime > n */
    seq_t an = next_prime_seq((seq_t)n);

    /* product = prime(n) * A151800(n) */
    seq_t prod = prime_n * an;

    /* a(n) = A151800(product) */
    return next_prime_seq(prod);
}
