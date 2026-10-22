/*
 * A332094  Numerator of the average distance among first n primes.
 * https://oeis.org/A332094
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( (2/(n*(n-1))) * Σ_{j=2..n} Σ_{i=1..j-1}
 *            (prime(j)-prime(i)) )
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_gcd, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the average distance among the first n primes.
   Uses the routine sieve_first_n_primes to obtain the first n primes. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_gcd.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;  /* undefined for n < 2; behaviour irrelevant */
    }

    /* Obtain the first n primes (array of uint32_t) */
    uint32_t *primes;
    size_t cnt;
    sieve_first_n_primes(n, &primes, &cnt);   /* cnt == n */

    seq_t prefix = 0;   /* sum of primes seen so far */
    seq_t sum   = 0;    /* cumulative sum of terms (j-1)*p_j - prefix */

    for (idx_t j = 1; j <= n; ++j) {
        seq_t pj = (seq_t)primes[j - 1];
        if (j >= 2) {
            seq_t term = ((seq_t)(j - 1)) * pj - prefix;
            sum += term;
        }
        prefix += pj;
    }

    /* Compute numerator = 2 * sum / GCD(2*sum, n*(n-1)) */
    seq_t num = sum * 2;
    seq_t den = (seq_t)n * (n - 1);
    seq_t g   = seq_gcd(num, den);
    num /= g;   /* den /= g;  // not needed for result */

    /* Clean up */
    free(primes);

    return num;
}
