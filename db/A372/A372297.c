/*
 * A372297  Limit of the recursion B(k) = T[k](B(k-1)), where B(1) =
 *          (1,2,3,4,5,...) and T[k] is the transformation that permutes the
 *          entries k(2i-1) and k(2i) for all positive integers i, if k is
 *          prime.
 * https://oeis.org/A372297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P^{-1}(n), where P is the permutation obtained by *
 *            swapping the entries k(2i-1) and k(2i) for all primes k (in
 *            ascending order) * applied to the identity sequence. The
 *            implementation computes a(n) by * iterating over primes ≤ n in
 *            descending order and applying the inverse * swaps to the t...
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

static uint64_t *primes = NULL;          /* list of primes */
static size_t primes_cnt = 0;            /* number of primes */
static idx_t   primes_limit = 0;         /* largest prime generated so far */

/* Generate all primes up to 'limit' (inclusive) using a simple sieve.
 * The array 'primes' is reallocated and filled with the primes.
 */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    size_t n = (size_t)(limit + 1);          /* array size needed */
    bool *is_prime = (bool *)malloc(n);
    if (!is_prime)
        exit(EXIT_FAILURE);

    memset(is_prime, true, n);
    if (n > 0) is_prime[0] = false;
    if (n > 1) is_prime[1] = false;

    size_t r = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= r; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t count = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            ++count;

    /* Allocate and fill the primes array */
    uint64_t *new_primes = (uint64_t *)calloc(count, sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            new_primes[idx++] = (uint64_t)i;

    free(is_prime);
    free(primes);

    primes      = new_primes;
    primes_cnt  = count;
    primes_limit = limit;
}

/* The requested OEIS sequence A372297. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of spec, but harmless */

    ensure_primes(n);
    seq_t pos = (seq_t)n;

    /* Apply primes in decreasing order: reverse the forward swaps. */
    for (size_t i = primes_cnt; i-- > 0; ) {
        uint64_t p = primes[i];
        if (pos % p == 0) {
            seq_t q = pos / p;
            if (q & 1ULL)
                pos += p;
            else
                pos -= p;
        }
    }
    return pos;
}
