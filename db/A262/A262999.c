/*
 * A262999  Number of ordered pairs (k, m) with k > 0 and m > 0 such that n =
 *          pi(k*(k+1)/2) + pi(m^2), where pi(x) denotes the number of primes
 *          not exceeding x.
 * https://oeis.org/A262999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(k,m) | k>0, m>0, n = pi(k(k+1)/2) + pi(m^2)}
 * Range:     u8 n <= 692, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A262999. */
/* It uses an adaptive sieve to provide the prime counting function pi(x). */

#include <stdlib.h>
#include <math.h>      /* for sqrt() */
#include <stdbool.h>   /* for bool, if needed */

static unsigned char *sieve = NULL;   /* 0/1 array marking primes */
static seq_t        *pi_arr = NULL;   /* prefix prime counts */
static idx_t        sieve_limit = 0;  /* largest number already sieved */

/* Ensure we have prime information up to at least n. */
static void
ensure_sieve(idx_t n)
{
    if (n <= sieve_limit)
        return;

    /* Allocate new arrays.  (We rebuild the sieve entirely; this is
       efficient for n up to a few hundred thousand, which covers all
       cases needed for the sequence.) */
    unsigned char *new_sieve = (unsigned char *)malloc((size_t)(n + 1));
    seq_t        *new_pi    = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    if (!new_sieve || !new_pi) {
        /* Allocation failed – abort.  In a real environment we would
           handle the error properly, but the BOI framework guarantees
           enough memory for these sizes. */
        exit(1);
    }

    /* Initialise all entries to prime, then clear 0 and 1. */
    for (idx_t i = 0; i <= n; ++i)
        new_sieve[i] = 1;
    if (n >= 0) new_sieve[0] = 0;
    if (n >= 1) new_sieve[1] = 0;

    /* Classic Sieve of Eratosthenes. */
    idx_t sq = (idx_t)sqrt((long double)n);
    for (idx_t i = 2; i <= sq; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                new_sieve[j] = 0;
        }
    }

    /* Build prefix sums for the prime counting function. */
    new_pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        new_pi[i] = new_pi[i - 1] + (seq_t)new_sieve[i];

    /* Replace old data. */
    free(sieve);
    free(pi_arr);
    sieve = new_sieve;
    pi_arr = new_pi;
    sieve_limit = n;
}

/* Prime counting function.  Returns pi(n). */
static inline seq_t
compute_pi(idx_t n)
{
    if (n == 0)
        return 0;
    ensure_sieve(n);
    return pi_arr[n];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    seq_t count = 0;

    for (idx_t k = 1; ; ++k) {
        idx_t triangular = k * (k + 1) / 2;
        seq_t pk = compute_pi(triangular);
        if (pk > target)
            break;                      /* further k will only increase pk */

        seq_t rem = target - pk;
        if (rem == 0) {
            /* Count m with pi(m^2) == 0.  m must be 1 in this case. */
            count += 1;                  /* k fixed, m=1 */
            continue;
        }

        /* Search for m such that pi(m^2) == rem.  Stop when pi(m^2) > rem. */
        for (idx_t m = 1; ; ++m) {
            idx_t msquare = m * m;
            seq_t pm = compute_pi(msquare);
            if (pm > rem)
                break;
            if (pm == rem)
                count += 1;
        }
    }

    return count;
}
