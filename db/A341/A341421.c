/*
 * A341421  a(n) is the number of k <= n for which (prime(k)+3*prime(k+1))/2
 *          is prime minus the number of k <= n for which
 *          (prime(k)+3*prime(k+2))/2 is prime.
 * https://oeis.org/A341421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = #{k ≤ n : (prime(k)+3*prime(k+1))/2 is prime} - #{k ≤ n :
 *            (prime(k)+3*prime(k+2))/2 is prime}
 * Range:     i8 n <= 10, i16 n <= 2678, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple sieve to obtain the first n+2 primes and tests
 * each candidate value only when the numerator is even, thus avoiding
 * false positives caused by integer division of odd sums. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/isPrimeGeneric.h"

static uint32_t *primes = NULL;     /* 1‑based indexing: primes[0] = prime(1) */
static size_t   primes_cnt = 0;     /* number of stored primes */

/* Return a(n) for the sequence A341421 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n < 1, but sub‑write 0 */

    /* We need primes up to prime(n+2) → n+2 primes */
    idx_t needed = n + 2;
    if (needed > (idx_t)primes_cnt) {
        if (primes)
            free(primes);
        primes = NULL;
        /* sieve_first_n_primes expects the count of primes, not the limit. */
        sieve_first_n_primes(needed, &primes, &primes_cnt);   /* primes[0] = prime(1) */
    }

    seq_t diff = 0;  /* running difference of counts */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t p_k   = (seq_t)primes[k-1];
        seq_t p_k1  = (seq_t)primes[k];     /* prime(k+1) */
        seq_t p_k2  = (seq_t)primes[k+1];   /* prime(k+2) */

        seq_t s1 = p_k + (seq_t)3 * p_k1;
        if ((s1 & 1) == 0) {                  /* even sum → integer candidate */
            seq_t val1 = s1 >> 1;             /* divide by 2 using shift */
            if (isPrimeGeneric(val1))
                ++diff;
        }

        seq_t s2 = p_k + (seq_t)3 * p_k2;
        if ((s2 & 1) == 0) {
            seq_t val2 = s2 >> 1;
            if (isPrimeGeneric(val2))
                --diff;
        }
    }
    return diff;
}
