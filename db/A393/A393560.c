/*
 * A393560  a(n) is the smallest prime p for which ((n+1)*p + 1)/n is also
 *          prime.
 * https://oeis.org/A393560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 2527, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = smallest prime p such that ((n+1)*p + 1)/n is also prime.
 * The code implements a direct search over primes p ≡ -1 (mod n) and tests
 * the derived number for primality.  It uses the generic prime test
 * `is_prime_big` supplied by the utilities. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* undefined below offset */
    seq_t n_seq = (seq_t)n;
    seq_t p_start;

    if (n == 1) {
        p_start = 2;                         /* start with the first prime */
    } else {
        p_start = (seq_t)(n - 1);            /* p ≡ n-1 (mod n) */
        if (p_start < 2) p_start += n_seq;   /* ensure p ≥ 2 */
    }

    for (seq_t p = p_start;; p += n_seq) {
        if (!is_prime_big(p)) continue;      /* p must be prime */

        /* compute q = ((n+1)*p + 1)/n ; guaranteed integer if p ≡ -1 mod n */
        seq_t num = (n_seq + 1) * p + 1;
        seq_t q = num / n_seq;

        if (is_prime_big(q)) return p;       /* found smallest qualifying prime */
    }
}
