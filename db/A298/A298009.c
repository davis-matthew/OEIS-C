/*
 * A298009  a(n) = f(n-1,n)+(n-1)*k, n>=1, where the function f(a,b) gives the
 *          number of prime numbers in the range [a*k,b*k[ with k=10^p. For
 *          this sequence we use p=2.
 * https://oeis.org/A298009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A298009:  
   a(n) = (# of primes in [100*(n-1), 100*n-1]) + 100*(n-1), n ≥ 1   */
/* The implementation uses a simple sieve of Eratosthenes and
   stores prime counts in a prefix array.  It expands lazily as
   larger arguments are required. */

#include <stdlib.h>
#include <stddef.h>

/*--- global state --------------------------------------------------------*/
static idx_t          max_limit   = 0;   /* largest upper bound already sieved */
static unsigned char *is_composite = NULL; /* 0 = prime (or 0/1), 1 = composite */
static idx_t *        pi_counts   = NULL; /* pi_counts[i] = #primes ≤ i */

/*--- helper: grow sieve up to 'limit' -------------------------------------*/
static void ensure_limit(idx_t limit)
{
    if (limit <= max_limit)
        return; /* already sufficient */

    /* grow the allocation */
    unsigned char *new_comp = (unsigned char *)realloc(is_composite,
                            (size_t)(limit + 1) * sizeof(unsigned char));
    idx_t *new_pi = (idx_t *)realloc(pi_counts,
                              (size_t)(limit + 1) * sizeof(idx_t));
    if (!new_comp || !new_pi) {
        /* allocation failed – abort */
        exit(1);
    }
    is_composite = new_comp;
    pi_counts    = new_pi;

    /* initialise freshly allocated part */
    for (idx_t i = max_limit + 1; i <= limit; ++i)
        is_composite[i] = 0;

    /* sieve 2 .. limit */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    /* build prefix prime counts */
    idx_t cnt = 0;
    pi_counts[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (i >= 2 && !is_composite[i])
            ++cnt;
        pi_counts[i] = cnt;
    }

    max_limit = limit;
}

/*--- sequence function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; guard against illegal call */
        return 0;

    /* bounds for the interval [100*(n-1), 100*n-1] */
    idx_t lower = 100ULL * (n - 1);          /* inclusive */
    idx_t upper_excl = 100ULL * n;          /* exclusive, so we need up to upper-1 */

    ensure_limit(upper_excl - 1);   /* guarantee data for all indices needed */

    /* number of primes in the interval */
    idx_t prime_before_lower = (lower == 0) ? 0 : pi_counts[lower - 1];
    idx_t prime_cup = pi_counts[upper_excl - 1] - prime_before_lower;

    /* final formula: primes + 100*(n-1) */
    return (seq_t)prime_cup + (seq_t)(100ULL * (n - 1));
}
