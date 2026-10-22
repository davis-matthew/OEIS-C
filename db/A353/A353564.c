/*
 * A353564  Product_{d|n, d<n} A276086(phi(d)), where A276086 is primorial
 *          base exp-function, and phi is Euler totient function.
 * https://oeis.org/A353564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A353564: a(n) = Product_{d|n, d<n} A276086(phi(d))
   where A276086 is the primorial base exponent-function.

   A276086(m) is computed by expressing m in primorial base
   (radical base with primes 2,3,5,7,…) and taking the
   product of the primes raised to the corresponding digits.
   For m>0: let m = Σ_{k>=1} d_k * P_{k-1} with 0 ≤ d_k < p_k
   (P_{k} = Π_{i=1}^{k} p_i).  
   Then A276086(m) = Π_{k>=1} p_k^{d_k}.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* global data */
static idx_t max_n = 0;
static idx_t *phi_arr = NULL;   /* Euler's totient, 1‑based */
static seq_t *f_arr   = NULL;   /* A276086(phi(i)) values */

/* small list of primes used in primorial base conversion */
static const uint32_t primes[] = {
    2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199           /* enough for m <= 10000 */
};
static const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

/* ensure computed data for all n <= needed */
static void ensure_computed(idx_t needed)
{
    if (needed <= max_n)
        return;

    /* allocate new arrays */
    size_t new_size = needed + 1;
    if (phi_arr) free(phi_arr);
    if (f_arr)   free(f_arr);
    phi_arr = (idx_t *)malloc(new_size * sizeof(idx_t));
    f_arr   = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!phi_arr || !f_arr)
        exit(1);                     /* out of memory */

    /* sieve for Euler's totient up to needed */
    for (idx_t i = 1; i <= needed; ++i)
        phi_arr[i] = i;
    for (idx_t p = 2; p <= needed; ++p) {
        if (phi_arr[p] == p) {       /* p is prime */
            for (idx_t m = p; m <= needed; m += p)
                phi_arr[m] -= phi_arr[m] / p;
        }
    }

    /* compute f_arr[m] = A276086(m) for all 1 <= m <= needed */
    for (idx_t m = 1; m <= needed; ++m) {
        seq_t res = 1;
        idx_t rem = m;
        idx_t k   = 0;
        idx_t prodP = 1;            /* P_{k-1} */
        while (rem > 0 && k < prime_count) {
            uint32_t p = primes[k];
            idx_t d     = (rem / prodP) % p;
            if (d > 0) {
                res *= big_pow((seq_t)p, (seq_t)d);
            }
            rem   -= d * prodP;
            prodP *= p;
            ++k;
        }
        f_arr[m] = res;
    }

    max_n = needed;
}

/*------------------------------------------------------------*/
/*  SEQUENCE FUNCTION                                        */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not defined, but return 0 */
    if (n == 1) return 1;        /* empty product */

    ensure_computed(n);

    seq_t result = 1;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 < n)
                result *= f_arr[phi_arr[d1]];
            if (d2 != d1 && d2 < n)
                result *= f_arr[phi_arr[d2]];
        }
    }
    return result;
}
