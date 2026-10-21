/*
 * A294109  Sum of the larger parts of the partitions of n into two parts with
 *          smaller part prime.
 * https://oeis.org/A294109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..⌊n/2⌋} (n-i) * c(i) = n * π(⌊n/2⌋) - Σ_{p ≤
 *            ⌊n/2⌋, p prime} p where c(i)=1 iff i is prime
 * Range:     u8 n <= 41, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static data for a prime sieve and cumulative tables */
static unsigned char *is_prime   = NULL;   /* 0/1 flags */
static seq_t      *pref_count   = NULL;   /* number of primes ≤ i */
static seq_t      *pref_sum     = NULL;   /* sum of primes ≤ i */
static idx_t      sieve_size   = 0;        /* current maximum index + 1 */

/* Build or extend the sieve and prefix tables up to at least 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit + 1 <= sieve_size)
        return;   /* nothing to do */

    /* Reallocate arrays */
    unsigned char *new_is   = (unsigned char *)realloc(is_prime,
                               (size_t)(limit + 1) * sizeof(unsigned char));
    seq_t        *new_cnt   = (seq_t *)realloc(pref_count,
                               (size_t)(limit + 1) * sizeof(seq_t));
    seq_t        *new_sum   = (seq_t *)realloc(pref_sum,
                               (size_t)(limit + 1) * sizeof(seq_t));

    /* In an unlikely event of realloc failure keep old data,
       but the build environment guarantees enough memory. */
    is_prime = new_is;
    pref_count = new_cnt;
    pref_sum = new_sum;

    /* Initialise all entries as “prime”, except 0 and 1 */
    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = (i >= 2) ? 1 : 0;

    /* Traditional Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Build prefix tables */
    pref_count[0] = (seq_t)0;
    pref_sum[0]   = (seq_t)0;
    if (limit >= 1) {
        pref_count[1] = (seq_t)0;
        pref_sum[1]   = (seq_t)0;
    }
    for (idx_t i = 2; i <= limit; ++i) {
        pref_count[i] = pref_count[i-1] + (seq_t)(is_prime[i] ? 1 : 0);
        pref_sum[i]   = pref_sum[i-1]   + (seq_t)(is_prime[i] ? i : 0);
    }

    sieve_size = limit + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t half = n >> 1;          /* floor(n/2) */
    ensure_sieve(half);

    seq_t prime_cnt = pref_count[half];
    seq_t prime_sum = pref_sum[half];

    return prime_cnt * (seq_t)n - prime_sum;
}
