/*
 * A299111  Maximum value of the cyclic convolution of first n primes with
 *          themselves.
 * https://oeis.org/A299111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k=1..n} Σ_{i=1..n} prime(n-i+1) * prime( (i+k) mod n
 *            + 1 ) (maximum value of the cyclic convolution of the first n
 *            primes with themselves)
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 667, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                   /* safety for out‑of‑range calls */

    uint32_t *primes = NULL;                 /* array of the first n primes */
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);  /* cnt == n */

    seq_t max_val = 0;

    for (idx_t k = 0; k < n; ++k) {
        seq_t sum = 0;
        for (idx_t i = 0; i < n; ++i) {
            idx_t rev_idx = n - 1 - i;          /* prime(n-i) in 0‑based */
            idx_t j = (i + k) % n;             /* shifted index */
            sum += (seq_t)primes[rev_idx] * (seq_t)primes[j];
        }
        if (k == 0 || sum > max_val) max_val = sum;
    }

    free(primes);
    return max_val;
}
