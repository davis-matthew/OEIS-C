/*
 * A309429  Least Luhn prime in base 2n: primes p such that p + reverse(p) in
 *          base 2n is also a prime.
 * https://oeis.org/A309429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   return the least prime p such that * p + reverse_{2n}(p) (in
 *            base 2n) is also prime * (Luhn prime for base 2n). The first
 *            term is p = 2 for n = 1
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Reverse the representation of x in the given base. */
static seq_t reverse_in_base(seq_t x, seq_t base)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t digit = x % base;
        rev = rev * base + digit;
        x /= base;
    }
    return rev;
}

/* Return the smallest prime >= start. */
static seq_t next_prime(seq_t start)
{
    if (start <= 2) return (seq_t)2;
    seq_t cand = start;
    if ((cand % 2) == 0) cand++;          /* make odd */
    while (!is_prime_big(cand)) {
        cand += 2;                        /* skip even numbers */
        /* no overflow check needed; wrap around will still retain correctness
           for the small builds where the result must fit the type */
    }
    return cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below OEIS offset */
    if (n == 1) return (seq_t)2;         /* only Luhn prime for base 2 */

    seq_t base  = (seq_t)n * (seq_t)2;            /* base 2n */
    seq_t start = (seq_t)8 * (seq_t)n * (seq_t)n; /* heuristic lower bound */

    seq_t p = next_prime(start);
    for (;;) {
        seq_t rev = reverse_in_base(p, base);
        seq_t sum = p + rev;
        if (is_prime_big(sum))
            return p;
        p = next_prime(p + (seq_t)1);        /* next prime after current p */
    }
}
