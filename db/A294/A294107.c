/*
 * A294107  Maximum of the number of primes appearing among the smaller parts
 *          and the number of primes appearing among the larger parts of the
 *          partitions of n into two parts.
 * https://oeis.org/A294107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( π(⌊n/2⌋) , π(n-1) – π(⌈n/2⌉-1) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where π(x) is the number of primes ≤ x. */

#include <stdlib.h>
#include <stdbool.h>

/* static prime sieve and prefix counts */
static bool *is_prime  = NULL; /* true if index is prime */
static seq_t *prime_cnt = NULL; /* prefix prime counts */
static idx_t capacity = 0;      /* current array size */

/* Ensure sieve data available up to at least `limit`. */
static void ensure_limit(idx_t limit)
{
    if (limit + 1 <= capacity) return;

    size_t new_cap = (size_t)(limit + 1);
    bool *new_prime  = (bool *)malloc(new_cap * sizeof(bool));
    if (!new_prime) return;
    for (size_t i = 0; i < new_cap; ++i) new_prime[i] = true;
    if (new_cap > 0) new_prime[0] = false;
    if (new_cap > 1) new_prime[1] = false;
    for (size_t p = 2; p * p < new_cap; ++p) {
        if (new_prime[p]) {
            for (size_t q = p * p; q < new_cap; q += p)
                new_prime[q] = false;
        }
    }

    seq_t *new_cnt = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cnt) {
        free(new_prime);
        return;
    }
    seq_t running = 0;
    for (size_t i = 0; i < new_cap; ++i) {
        if (new_prime[i]) running = running + (seq_t)1;
        new_cnt[i] = running;
    }

    free(is_prime);
    free(prime_cnt);
    is_prime  = new_prime;
    prime_cnt = new_cnt;
    capacity  = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined but harmless */

    idx_t limit = n - 1;                /* we need primes up to n-1 */
    ensure_limit(limit);

    idx_t k      = n / 2;               /* floor(n/2) */
    idx_t n_k    = n - k;               /* ceil(n/2) = n - floor(n/2) */
    idx_t idx_hi = n_k - 1;              /* we subtract primes ≤ ceil(n/2)-1 */

    seq_t low  = prime_cnt[k];          /* π(⌊n/2⌋)          */
    seq_t high = prime_cnt[limit] - prime_cnt[idx_hi];

    return (low > high) ? low : high;
}
