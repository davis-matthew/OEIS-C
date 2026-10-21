/*
 * A254205  Number of length 2 1..(n+2) arrays with no leading or trailing
 *          partial sum equal to a prime.
 * https://oeis.org/A254205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Count ordered pairs (a,b) with a,b∈[1..n+2] such that
 * a, b and a+b are not prime numbers.  A simple sieve of Eratosthenes
 * is performed up to 2*(n+2) and all admissible pairs are counted by a
 * double loop over non‑prime values. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Upper bounds: numbers run from 1 to N = n+2.
     * We need primes up to 2*N to test sums.                  */
    idx_t N   = n + 2;
    idx_t lim = 2 * N;                     /* inclusive upper bound */

    /* Allocate prime table.  Size is small (≤ 20004 for n ≤ 10000). */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(lim + 1));
    if (!is_prime) return 0;               /* allocation failure → 0 */

    /* Mark all as prime first. */
    for (idx_t i = 0; i <= lim; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= lim; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t mult = p * p; mult <= lim; mult += p)
            is_prime[mult] = 0;
    }

    /* Count admissible ordered pairs. */
    seq_t cnt = 0;
    for (idx_t a = 1; a <= N; ++a) {
        if (is_prime[a]) continue;          /* a must be non‑prime */
        for (idx_t b = 1; b <= N; ++b) {
            if (is_prime[b]) continue;      /* b must be non‑prime */
            if (is_prime[a + b]) continue;  /* sum must be non‑prime */
            cnt++;
        }
    }

    free(is_prime);
    return cnt;
}
