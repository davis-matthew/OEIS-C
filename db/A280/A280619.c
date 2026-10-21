/*
 * A280619  Integers m such that sigma(m) - eulerphi(m) <= 4*sqrt(m).
 * https://oeis.org/A280619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers m such that sigma(m) - φ(m) <= 4*sqrt(m);
 * This is equivalent to {1,8} union the set of primes union the set of
 * squares of primes.  The routine builds the sorted list of these
 * numbers lazily and returns the n‑th term (1‑based). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

static bool primes_inited = false;
static uint64_t *primes = NULL;       /* list of primes */
static size_t primes_count = 0;       /* number of primes */

static void init_primes(void)
{
    if (primes_inited) return;
    const size_t SIEVE_LIMIT = 2000000;   /* enough for n ≈ 10 000 */
    bool *is_prime = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;                /* allocation failed – unlikely */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= SIEVE_LIMIT; m += p)
                is_prime[m] = false;
        }
    }
    /* Estimate prime count: <= SIEVE_LIMIT / log SIEVE_LIMIT */
    primes_count = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes_count++;

    primes = (uint64_t *)malloc(primes_count * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
    primes_inited = true;
}

/* sequence cache ---------------------------------------------------- */
static seq_t *seq_val   = NULL;   /* cached values */
static size_t seq_cap   = 0;     /* allocated capacity */
static size_t seq_size  = 0;     /* number of values stored */

static size_t prime_idx   = 0;   /* next unused prime index */
static size_t square_idx  = 0;   /* next unused prime square index */
static bool eight_added = false; /* whether the special value 8 is inserted */

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of range, but never requested */
    if (!primes_inited) init_primes();

    if (seq_val == NULL) {
        seq_cap = 16;
        seq_val = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (seq_size < n) {
        seq_t next_val;
        if (seq_size == 0) {
            next_val = (seq_t)1;           /* first term is 1 */
        } else {
            unsigned long long cand_prime  = (prime_idx < primes_count)
                                             ? (unsigned long long)primes[prime_idx]
                                             : ULLONG_MAX;
            unsigned long long cand_square = (square_idx < primes_count)
                                             ? (unsigned long long)primes[square_idx] *
                                               (unsigned long long)primes[square_idx]
                                             : ULLONG_MAX;
            unsigned long long cand_eight  = eight_added ? ULLONG_MAX : 8ULL;

            unsigned long long tc = cand_prime;
            if (cand_square < tc) tc = cand_square;
            if (cand_eight < tc)  tc = cand_eight;

            next_val = (seq_t)tc;

            if (tc == cand_prime)      prime_idx++;
            else if (tc == cand_square) square_idx++;
            else                       eight_added = true;
        }

        if (seq_size >= seq_cap) {         /* grow cache if needed */
            seq_cap = seq_cap * 2;
            seq_val = (seq_t *)realloc(seq_val, seq_cap * sizeof(seq_t));
        }
        seq_val[seq_size++] = next_val;
    }

    return seq_val[n - 1];
}
