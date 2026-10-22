/*
 * A383037  a(n) is the excess of composites over primes in the first n odd
 *          positive integers.
 * https://oeis.org/A383037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 2*pi(2*n) + 1 where pi(x) is the number of primes ≤ x
 * Range:     i8 n <= 408, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- standard headers that are allowed --- */
#include <stdint.h>
#include <stdlib.h>

/* --- local prime counting routine ------------------------------------ */

/* Compute π(limit) – the number of primes ≤ limit.
 * A very small simple sieve is sufficient for the expected range of
 * n (tens of thousands).  The routine is correct for any limit that
 * fits in a 64‑bit integer. */
static seq_t compute_pi(idx_t limit)
{
    if (limit < 2)
        return (seq_t)0;

    /* Allocate an array of booleans (bytes).  0 means composite,
     * 1 means probable prime. */
    uint8_t *is_prime = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (is_prime == NULL)
        exit(EXIT_FAILURE);      /* memory allocation failed */

    /* 0 and 1 are not prime.  Mark the rest as prime initially. */
    is_prime[0] = 0;
    is_prime[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    /* Eratosthenes sieve */
    for (idx_t p = 2; p <= limit / p; ++p)   /* p*p <= limit, avoid overflow */
    {
        if (is_prime[p])
        {
            idx_t start = p * p;              /* p=p*p  <= limit  */
            for (idx_t j = start; j <= limit; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    seq_t count = (seq_t)0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    free(is_prime);
    return count;
}

/* --- sequence implementation ---------------------------------------- */

seq_t a(idx_t n)
{
    /* 2*n, using unsigned arithmetic on idx_t */
    idx_t two_n = n << 1;

    /* π(2*n) */
    seq_t pi_val = compute_pi(two_n);

    /* a(n) = n - 2*π(2*n) + 1 */
    return (seq_t)n - ((seq_t)2 * pi_val) + 1;
}
