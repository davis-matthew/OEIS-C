/*
 * A356094  a(n) = denominator((prime(n)-1)/prime(n)#), where prime(n)# =
 *          A002110(n) is the n-th primorial.
 * https://oeis.org/A356094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((prime(n)-1)/prime(n)#)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   seq_mul, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                           
   prime(n)# is the product of all primes ≤ prime(n).  
   The denominator after reducing the fraction equals the product of all
   primes ≤ prime(n) that do NOT divide prime(n)-1. */

#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* not used, but safe */

    /* -------------------------------------------------------------
       Cache the first N primes so that multiple calls with varying
       n do not recompute the sieve each time.
       ------------------------------------------------------------- */
    static uint32_t *prime_list = NULL;
    static size_t cached_n   = 0;   /* number of primes cached */

    if (n > cached_n)
    {
        /* Free any previously allocated array */
        if (prime_list) free(prime_list);

        uint32_t *new_list = NULL;
        size_t    new_cnt  = 0;

        /* build_sieve_first_n_primes allocates its own array that must
           be freed by the caller.                                   */
        sieve_first_n_primes(n, &new_list, &new_cnt);

        prime_list = new_list;
        cached_n   = new_cnt;   /* new_cnt should equal n */
    }

    /* nth prime: array is 0‑based */
    uint32_t p_n = prime_list[n - 1];

    /* numerator of the fraction: (prime(n)-1) */
    seq_t rem = (seq_t)p_n - (seq_t)1;

    seq_t denom = (seq_t)1;

    /* denominator equals product of primes ≤ p_n that do NOT divide rem */
    for (size_t i = 0; i < (size_t)n; ++i)
    {
        seq_t q = (seq_t)prime_list[i];
        if (rem % q != (seq_t)0)
            denom = seq_mul(denom, q);
    }

    return denom;
}
