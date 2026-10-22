/*
 * A307857  Number of partitions of n into 1, 2 or 3 nonprime parts.
 * https://oeis.org/A307857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c(n) + Σ_{i=1..⌊n/2⌋} c(i)c(n−i) + Σ_{j=1..⌊n/3⌋}
 *            Σ_{i=j..⌊(n−j)/2⌋} c(i)c(j)c(n−i−j) where c(k) = number of
 *            nonprime integers ≤ k (A005171)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static bool *is_prime_tbl = NULL;        /* 1 if prime, 0 if nonprime (except 0,1) */
static seq_t *prefix_nonprime = NULL;    /* prefix_nonprime[k] = c(k) */
static idx_t sieve_max = 0;              /* largest n for which tables are valid */

/* Allocate and compute a sieve and prefix sums up to n. */
static void ensure_sieve(idx_t n)
{
    if (n <= sieve_max)
        return;            /* already computed */

    /* Allocate new tables */
    bool *tbl   = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    seq_t *pref = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tbl || !pref) return;   /* allocation failed – caller gets 0 */

    /* Initialise: assume all are prime initially */
    for (idx_t i = 0; i <= n; ++i) tbl[i] = true;
    if (n >= 0) tbl[0] = false;
    if (n >= 1) tbl[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; (p * p) <= n; ++p)
        if (tbl[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                tbl[m] = false;
        }

    /* Compute prefix sums of nonprime counts */
    seq_t count = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (!tbl[i]) ++count;    /* 0 and 1 included as nonprime */
        pref[i] = count;
    }

    /* Free previous tables */
    free(is_prime_tbl);
    free(prefix_nonprime);

    /* Save new tables */
    is_prime_tbl = tbl;
    prefix_nonprime = pref;
    sieve_max = n;
}

/* Returns 1 if number <= n is nonprime, 0 if prime. Assumes n <= sieve_max. */
static inline bool is_nonprime(idx_t n)
{
    return !is_prime_tbl[n];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    ensure_sieve(n);

    seq_t result = 0;

    /* One-part partitions: just the number itself if nonprime. */
    if (is_nonprime(n))
        ++result;

    /* Two-part partitions: sums i + (n-i) with 1 ≤ i ≤ floor(n/2). */
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (is_nonprime(i) && is_nonprime(n - i))
            ++result;
    }

    /* Three-part partitions: i + j + k = n with i ≥ j ≥ 1, k ≥ 1. */
    for (idx_t j = 1; j <= n / 3; ++j) {
        if (!is_nonprime(j))
            continue;       /* j must be nonprime */
        idx_t max_i = (n - j) / 2;
        for (idx_t i = j; i <= max_i; ++i) {
            if (!is_nonprime(i))
                continue;
            idx_t k = n - i - j;
            if (k < 1) continue;
            if (is_nonprime(k))
                ++result;
        }
    }

    return result;
}
