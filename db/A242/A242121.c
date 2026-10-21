/*
 * A242121  a(n) = n-th prime modulo 21.
 * https://oeis.org/A242121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242121: a(n) = n-th prime modulo 21, offset 1. */
#include <stdlib.h>
#include <stdbool.h>

/* Ensure that the first `cnt` primes are available in the static array `primes`.  */
static uint32_t *primes = NULL;   /* array of primes */
static size_t prime_count = 0;    /* number of primes in the array */

/* Build (or rebuild) the prime list up to a limit that surely contains at least `cnt` primes. */
static void build_up_to_n_primes(size_t cnt)
{
    /* A simple upper bound: for n ≥ 1, p_n < 20*n works for n ≤ 10000 (gives 200 000). */
    size_t limit = (cnt > 0) ? (cnt * 20 + 10) : 10;
    bool *is_prime = (bool*)malloc(limit + 1);
    if (!is_prime) return; /* out of memory – silently give up (should not happen here) */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t mul = p * p; mul <= limit; mul += p) {
                is_prime[mul] = false;
            }
        }
    }

    /* Count how many primes we found */
    size_t found = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++found;

    /* Allocate or resize the primes array */
    if (prime_count < found) {
        uint32_t *newp = (uint32_t*)realloc(primes, found * sizeof(uint32_t));
        if (!newp) {
            free(is_prime);
            return; /* out of memory – leave old array intact */
        }
        primes = newp;
        prime_count = found;
    }

    /* Fill the array with the primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit && idx < prime_count; ++i) {
        if (is_prime[i]) {
            primes[idx++] = (uint32_t)i;
        }
    }

    free(is_prime);
}

/* Return the n-th prime modulo 21 (with a(1) = first prime). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside the sequence offset – undefined behaviour is forbidden */
    size_t u = (size_t)n;

    if (prime_count < u) build_up_to_n_primes(u);

    uint32_t p = primes[u - 1];
    return (seq_t)(p % 21ull);
}
