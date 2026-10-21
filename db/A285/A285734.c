/*
 * A285734  a(1) = 0, and for n > 1, a(n) = the largest squarefree number x
 *          such that x < n-x, and n-x is also squarefree.
 * https://oeis.org/A285734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = largest squarefree x ≤ n/2 such that n‑x is also squarefree */

/* required types are provided by the build */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* local integer square root (works for 64‑bit values) */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (idx_t)1ULL << 32;   /* (1<<32)^2 = 1<<64 > max 64‑bit */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* cache for squarefree flags */
static bool  *sqfree      = NULL;   /* sqfree[i] == true ⇔ i is squarefree */
static idx_t sqfree_size = 0ULL;    /* number of entries in sqfree (= limit+1) */

/* initialise or extend the squarefree array up to `limit` (inclusive) */
static void init_sqfree(idx_t limit) {
    idx_t sqrt_lim = isqrt_u64(limit);

    /* sieve to find primes up to sqrt_lim */
    bool *is_prime = (bool *)malloc((sqrt_lim + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (idx_t i = 0; i <= sqrt_lim; ++i) is_prime[i] = true;
    if (sqrt_lim >= 0) is_prime[0] = false;
    if (sqrt_lim >= 1) is_prime[1] = false;
    for (idx_t p = 2; p <= sqrt_lim; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= sqrt_lim; j += p)
                is_prime[j] = false;

    /* allocate and initialise the squarefree array */
    bool *new_sqfree = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!new_sqfree) abort();
    for (idx_t i = 0; i <= limit; ++i) new_sqfree[i] = true;

    /* mark numbers divisible by a square > 1 */
    for (idx_t p = 2; p <= sqrt_lim; ++p)
        if (is_prime[p]) {
            idx_t sq = p * p;
            for (idx_t j = sq; j <= limit; j += sq)
                new_sqfree[j] = false;
        }

    /* replace the old array */
    free(sqfree);
    sqfree = new_sqfree;
    sqfree_size = limit + 1;

    free(is_prime);
}

/* compute a(n) for any n ≥ 1 */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    /* ensure we have squarefree information up to n */
    if (n >= sqfree_size) init_sqfree(n);

    idx_t half = n / 2;                /* floor(n/2) */
    for (idx_t x = half; x >= 1; --x) {
        if (sqfree[x] && sqfree[n - x])
            return (seq_t)x;
    }
    return (seq_t)0;   /* should never happen for n > 1 */
}
