/*
 * A347788  Number of compositions (ordered partitions) of n into at most 2
 *          nonprime parts.
 * https://oeis.org/A347788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of compositions of n into at most 2 nonprime parts
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (direct counting; see OEIS A347788) */

#include <stdlib.h>
#include <string.h>

/* --------------  helper: prime sieve -------------- */
static char *prime_sieve = NULL;   /* 1 = prime, 0 = nonprime/0/1 */
static size_t sieve_limit = 0;     /* size of prime_sieve array */

/* expand the sieve up to at least n (inclusive) */
static void ensure_sieve(idx_t n)
{
    size_t target = (size_t)n + 1; /* need indices 0..n */
    if (target <= sieve_limit)
        return;                    /* already big enough */

    /* allocate new sieve */
    char *new_sieve = (char *)calloc(target, sizeof(char));
    if (!new_sieve)
        exit(1);                   /* fatal allocation error */

    /* initialise: mark everything as prime */
    for (size_t i = 0; i < target; ++i)
        new_sieve[i] = 1;
    if (target > 0) new_sieve[0] = 0; /* 0 is not prime */
    if (target > 1) new_sieve[1] = 0; /* 1 is not prime */

    /* Sieve of Eratosthenes */
    for (size_t p = 2; p * p < target; ++p)
    {
        if (new_sieve[p])
        {
            for (size_t m = p * p; m < target; m += p)
                new_sieve[m] = 0;
        }
    }

    /* replace old sieve */
    free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = target;
}

/* --------------  main sequence function -------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* empty composition */

    ensure_sieve(n);

    seq_t count = 0;

    /* single‑part composition (n itself) */
    if (!prime_sieve[(size_t)n])
        count += (seq_t)1;

    /* two‑part compositions: (a, b) with a+b = n, a,b>0 */
    for (idx_t a_part = 1; a_part < n; ++a_part)
    {
        idx_t b_part = n - a_part;
        if (!prime_sieve[(size_t)a_part] && !prime_sieve[(size_t)b_part])
            count += (seq_t)1;
    }

    return count;
}
