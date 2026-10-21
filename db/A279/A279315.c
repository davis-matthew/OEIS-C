/*
 * A279315  Count the primes appearing in each interval [p,q] where (p,q) is a
 *          Goldbach partition of 2n such that all primes from p to q
 *          (inclusive) appear as a part in some Goldbach partition of p+q =
 *          2n, and then add the results.
 * https://oeis.org/A279315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Count the primes appearing in each interval [p,q] where
 *            (p,q) is a Goldbach partition of 2n such that all primes from p
 *            to q (inclusive) appear as a part in some Goldbach partition of
 *            p+q = 2n, and then add the results. (This implementation follows
 *            the main expression from the OEIS entry.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool *prime_flags   = NULL;   /* prime_flags[i] == true iff i is prime */
static seq_t *pi_values    = NULL;   /* π(i) for 0≤i≤prime_limit            */
static idx_t  prime_limit  = 0;      /* highest number sieved              */

/* Build a prime sieve and Pi table up to 'limit' (inclusive). */
static void
sieve_and_pi(idx_t limit)
{
    /* Allocate and zero the flags array. */
    bool *flags = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!flags) exit(1);                   /* out of memory */

    /* Assume all numbers ≥2 are prime initially. */
    for (idx_t i = 2; i <= limit; ++i) flags[i] = true;

    /* Eratosthenes.  Use idx_t for the loop counter. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!flags[p]) continue;
        for (idx_t m = p * p; m <= limit; m += p) flags[m] = false;
    }

    /* Build the Pi table. */
    seq_t *pi = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!pi) { free(flags); exit(1); }
    pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        pi[i] = pi[i - 1] + (flags[i] ? (seq_t)1 : (seq_t)0);

    /* Replace old tables. */
    free(prime_flags);
    free(pi_values);
    prime_flags = flags;
    pi_values   = pi;
    prime_limit = limit;
}

/* Ensure the tables reach at least 'need'. */
static void
ensure_prime_pi(idx_t need)
{
    if (need <= prime_limit) return;
    sieve_and_pi(need);
}

/* The sequence function. */
seq_t
a(idx_t n)
{
    /* The first two terms are zero. */
    if (n < 3) return 0;

    idx_t two_n = 2 * n;

    /* We need information up to two_n. */
    ensure_prime_pi(two_n);

    /* diff[k] is true if c(k) != c(2n-k) (k from 3 to n). */
    bool *diff = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!diff) exit(1);

    for (idx_t k = 3; k <= n; ++k)
        diff[k] = prime_flags[k] != prime_flags[two_n - k];

    /* pref[k] = number of mismatches in [3..k]. */
    idx_t *pref = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!pref) { free(diff); exit(1); }

    pref[0] = 0;
    for (idx_t k = 1; k <= n; ++k)
        pref[k] = pref[k - 1] + (diff[k] ? 1 : 0);

    seq_t result = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* (i, two_n-i) must both be prime. */
        if (!prime_flags[i] || !prime_flags[two_n - i]) continue;

        /* Check symmetry condition: no mismatch in [i..n]. */
        if (pref[n] - pref[i - 1] != 0) continue;

        /* Count primes in [i, two_n-i].  π(t)-π(l-1). */
        seq_t add = pi_values[two_n - i] - pi_values[i - 1];
        result += add;
    }

    free(diff);
    free(pref);
    return result;
}
