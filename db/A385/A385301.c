/*
 * A385301  a(n) = Sum_{k=0..p-1} 1/k! mod p where p is prime(n) and 1/k! is
 *          the inverse of k! modulo p.
 * https://oeis.org/A385301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..prime(n)-1} 1/k! mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 2585, u32 n <= 2585, u64 n <= 2585,
 *            u128 n <= 2585, big any n
 * Helpers:   get_prime, mod_mul, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    /* get the n-th prime */
    uint64_t p = get_prime(n);
    if (p == 0) return 0;          /* safety guard */

    seq_t mod = (seq_t)p;          /* modulo value */
    size_t len = (size_t)p;        /* number of terms in sum (p) */

    /* Allocate array for factorials modulo p */
    seq_t *fact = (seq_t *)malloc(len * sizeof(seq_t));
    if (!fact) return 0;           /* allocation failure */

    /* Compute factorials modulo p */
    fact[0] = 1 % mod;
    for (size_t k = 1; k < len; ++k) {
        fact[k] = (seq_t)mod_mul((uint64_t)fact[k - 1], (uint64_t)k,
                                (uint64_t)mod);
    }

    /* Compute inverse factorial of (p-1)! via Fermat */
    seq_t inv = (seq_t)mod_pow(fact[len - 1], p - 2, mod);  /* inv[p-1] */

    /* Accumulate the sum of inverse factorials */
    uint64_t acc = 0;
    acc += (uint64_t)inv;
    if (acc >= (uint64_t)mod) acc -= (uint64_t)mod;

    /* Backward loop: inv[k-1] = inv[k] * k (mod p) */
    for (size_t k = len - 1; k > 0; --k) {
        inv = (seq_t)mod_mul((uint64_t)inv, (uint64_t)k,
                             (uint64_t)mod);   /* inv[k-1] */
        acc += (uint64_t)inv;
        if (acc >= (uint64_t)mod) acc -= (uint64_t)mod;
    }

    free(fact);
    return (seq_t)(acc % (uint64_t)mod);
}
