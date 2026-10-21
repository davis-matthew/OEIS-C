/*
 * A242084  5^p - 4^p - 1, where p is prime.
 * https://oeis.org/A242084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 5^p(n) - 4^p(n) - 1, where p(n) is the nth prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                 /* For uint32_t */
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    if (n < 1)  /* undefined for n < 1 – behaviour below offset does not matter */
        return (seq_t)0;

    /* obtain the nth prime (1‑based indexing) */
    uint32_t p = get_prime(n);

    /* compute 5^p and 4^p using the arbitrary‑precision big_pow routine */
    seq_t five_pow  = big_pow((seq_t)5, (seq_t)p);
    seq_t four_pow  = big_pow((seq_t)4, (seq_t)p);

    /* return the result: 5^p - 4^p - 1 */
    return five_pow - four_pow - (seq_t)1;
}
