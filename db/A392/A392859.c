/*
 * A392859  Multiplicative inverse of {A143293(n) modulo prime(1+n)} in the
 *          finite field F(prime(1+n)), or 0 if prime(1+n) divides A143293(n),
 *          where A143293 gives the partial sums of primorials.
 * https://oeis.org/A392859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A143293(n))^{-1} (mod prime(n+1)), where A143293(n) =
 *            sum_{k=0}^n primorial_k, primorial_0 = 1, primorial_k (k>0) =
 *            primorial_{k-1} * prime_k. If prime(n+1) divides A143293(n),
 *            then a(n) = 0
 * Range:     u8 n <= 53, u16 n <= 4827, u32 n <= 6719, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* prime_{n+1} */
    seq_t prime = (seq_t)get_prime(n + 1);

    /* partial sum of primorials modulo prime */
    seq_t sum  = ((seq_t)1) % prime;   /* primorial_0 = 1 */
    seq_t prod = ((seq_t)1) % prime;   /* running product for primorial */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t p = (seq_t)get_prime(i);       /* prime_i   (1‑indexed) */
        prod = (prod * p) % prime;           /* primorial_i */
        sum  = (sum + prod) % prime;         /* add to partial sum */
    }

    if (sum == (seq_t)0)
        return (seq_t)0;                     /* division by zero case */

    /* Fermat's little theorem to compute the inverse modulo a prime */
    uint64_t exp = (uint64_t)(prime - (seq_t)2);
    return mod_pow(sum, exp, prime);
}
