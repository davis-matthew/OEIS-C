/*
 * A392862  a(n) = A143293(n)+14 mod prime(n+1), where A143293 gives the
 *          partial sums of primorials.
 * https://oeis.org/A392862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A143293(n)+14) mod prime(n+1)
 * Range:     u8 n <= 53, u16 n <= 4910, u32 n <= 7979, u64 n <= 9048,
 *            u128 n <= 9048, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"

/* Computes a(n) = (sum_{k=0}^n primorial(k) + 14) mod prime(n+1),
   where primorial(k) = product of the first k primes (with primorial(0)=1). */
seq_t a(idx_t n)
{
    /* Prime associated with this n (prime(n+1)) */
    seq_t mod = (seq_t)get_prime(n + 1);

    /* Running sum of primorials modulo mod */
    seq_t sum_mod = 0;

    /* Current primorial value modulo mod (starts with primorial(0)=1) */
    seq_t product = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t prim;
        if (k == 0) {
            prim = 1;
        } else {
            /* Update primorial: multiply by the next prime (k-th prime) */
            product = (product * (seq_t)get_prime(k)) % mod;
            prim = product;
        }
        /* Add to the running sum modulo mod */
        sum_mod = (sum_mod + prim) % mod;
    }

    /* Final result: add 14 and reduce modulo mod */
    return (sum_mod + 14) % mod;
}
