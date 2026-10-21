/*
 * A268605  a(1) = 0; a(n+1) is the smallest integer in which the difference
 *          between its digits sum and the a(n) digits sum is equal to the
 *          n-th prime.
 * https://oeis.org/A268605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer whose digit sum equals the sum * of the
 *            first (n‑1) primes. For n = 1 the sum is 0, hence a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   pow10, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Caching structures visible only inside this translation unit */
    static uint32_t *primes      = NULL;   /* array of primes */
    static size_t      prime_count = 0;     /* number of primes stored */
    static seq_t *cum_sums       = NULL;   /* cumulative sums, cum_sums[0] = 0 */
    static size_t      cum_sums_size = 0;   /* current capacity of cum_sums */

    /* initialise cumulative sums buffer */
    if (cum_sums == NULL) {
        cum_sums = (seq_t *)malloc(sizeof(seq_t));
        if (cum_sums == NULL) return (seq_t)0;  /* out of memory – ignore */
        cum_sums[0] = (seq_t)0;
        cum_sums_size = 1;
    }

    /* n is 1‑based.  We need the sum of the first (n‑1) primes. */
    idx_t needed = (n > 0) ? n - 1 : 0;
    if (needed > prime_count) {
        /* Re­gener­ate the first 'needed' primes */
        uint32_t *new_primes = NULL;
        size_t     new_count = 0;
        sieve_first_n_primes(needed, &new_primes, &new_count);
        /* Replace old prime table */
        if (primes) free(primes);
        if (cum_sums) free(cum_sums);
        primes      = new_primes;
        prime_count = new_count;
        /* Build new cumulative sum array */
        cum_sums_size = prime_count + 1;
        cum_sums = (seq_t *)malloc(sizeof(seq_t) * cum_sums_size);
        if (cum_sums == NULL) return (seq_t)0;  /* out of memory – ignore */
        cum_sums[0] = (seq_t)0;
        for (size_t i = 0; i < prime_count; ++i)
            cum_sums[i + 1] = cum_sums[i] + (seq_t)primes[i];
    }

    /* sum of the first (n-1) primes */
    seq_t sum = cum_sums[needed];

    /* minimal integer with digit sum 'sum' */
    seq_t q = sum / (seq_t)9;                 /* number of trailing 9's   */
    unsigned r = (unsigned)(sum % (seq_t)9);  /* leading digit (0..8)     */

    seq_t pow10q = pow10((idx_t)q);
    seq_t result;

    if (r == 0) {
        /* number consists only of q trailing 9s */
        result = pow10q - (seq_t)1;          /* 10^q - 1 -> 999…9   */
    } else {
        /* leading digit r followed by q nines */
        result = (seq_t)r * pow10q + (pow10q - (seq_t)1);
    }
    return result;
}
