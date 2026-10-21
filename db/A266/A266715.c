/*
 * A266715  Number of k <= floor(n/2) such that (n mod k) is prime.
 * https://oeis.org/A266715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ k : 1 ≤ k ≤ ⌊n/2⌋, (n mod k) is prime }|
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Precompute primes up to 5000 (max floor(n/2) for n ≤ 10000). */
static bool prime[5001] = { false };
static bool sieve_done = false;
static idx_t sieve_limit = 0;

/* Helper: build sieve up to `limit`. */
static void build_sieve(idx_t limit) {
    if (limit <= sieve_limit) return;
    sieve_limit = limit;
    /* Clear array up to limit. */
    memset(prime, true, (size_t)(limit + 1) * sizeof(bool));
    if (limit >= 0) prime[0] = false;
    if (limit >= 1) prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                prime[m] = false;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined for n < 1, but return 0 */

    idx_t limit = n / 2;            /* floor(n/2) */
    if (limit == 0) return 0;       /* a(1) = 0, a(2) = 0, ... */

    /* Ensure primes up to limit are ready. */
    if (!sieve_done) {
        build_sieve(limit);
        sieve_done = true;
    } else {
        if (limit > sieve_limit)
            build_sieve(limit);
    }

    seq_t count = 0;
    for (idx_t k = 1; k <= limit; ++k) {
        idx_t r = n % k;
        if (r > 1 && r <= limit && prime[r])
            ++count;
    }
    return count;
}
