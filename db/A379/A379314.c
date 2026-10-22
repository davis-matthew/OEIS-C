/*
 * A379314  Number of integer partitions of n with a unique 1 or prime part.
 * https://oeis.org/A379314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 82, u32 n <= 223, u64 n <= 672,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Computes sequence A379314: number of integer partitions of n in which
   exactly one part is equal to 1 or to a prime, and all other parts are
   composite. The function uses dynamic programming on composites and
   then sums the partitions that contain the unique 1 or the unique prime. */

static seq_t *f = NULL;          /* partitions of a number into composite parts */
static size_t f_cap = 0;          /* allocated size of f */
static uint64_t *primes = NULL;  /* list of primes up to current limit */
static size_t primes_cap = 0;    /* allocated size of primes array */
static size_t primes_cnt = 0;    /* number of primes stored */
static idx_t max_calculated = 0; /* largest n for which f and primes are valid */

/* Recompute tables up to new_limit.  The routine is O(new_limit^2). */
static void recompute(idx_t new_limit)
{
    /* --- ensure f array has enough space --- */
    size_t cap_f = (size_t)new_limit + 1;
    if (f_cap < cap_f) {
        seq_t *tmp = (seq_t *)realloc(f, cap_f * sizeof(seq_t));
        if (!tmp) abort();
        f = tmp;
        f_cap = cap_f;
    }

    /* --- sieve primes up to new_limit --- */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)new_limit + 1);
    if (!is_prime) abort();
    memset(is_prime, 1, (size_t)new_limit + 1);
    if (new_limit >= 0) is_prime[0] = 0;
    if (new_limit >= 1) is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= (uint64_t)new_limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= (uint64_t)new_limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* build primes list */
    if (primes_cap < (size_t)new_limit + 1) {
        uint64_t *tmp = (uint64_t *)realloc(primes, ((size_t)new_limit + 1) * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = (size_t)new_limit + 1;
    }
    primes_cnt = 0;
    for (uint64_t i = 2; i <= (uint64_t)new_limit; ++i) {
        if (is_prime[i])
            primes[primes_cnt++] = i;
    }

    /* --- compute f array: partitions into composites only --- */
    for (size_t i = 0; i <= (size_t)new_limit; ++i)
        f[i] = 0;
    f[0] = 1;   /* empty partition of 0 */

    for (uint64_t c = 4; c <= (uint64_t)new_limit; ++c) {      /* composites start at 4 */
        if (is_prime[c]) continue;         /* skip primes and 1,2,3 */
        for (uint64_t s = c; s <= (uint64_t)new_limit; ++s) {
            f[(size_t)s] += f[(size_t)(s - c)];
        }
    }

    free(is_prime);
    max_calculated = new_limit;
}

seq_t a(idx_t n)
{
    if (n > max_calculated)
        recompute(n);

    seq_t ans = 0;

    /* contributions from a unique 1 */
    if (n >= 1)
        ans += f[(size_t)(n - 1)];

    /* contributions from a unique prime part */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > (uint64_t)n)
            break;
        ans += f[(size_t)(n - p)];
    }

    return ans;
}
