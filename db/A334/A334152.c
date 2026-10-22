/*
 * A334152  Number of Goldbach partitions (p,q) of 2n, such that p-2 and q-2
 *          are both prime or both not prime.
 * https://oeis.org/A334152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} [c(i-2)=c(2*n-i-2)] * c(i) * c(2*n-i)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where c(x) is 1 if x is prime, otherwise 0. */
/* The implementation uses a simple sieve to test primality of numbers
   up to 2*n and counts unordered Goldbach partitions that satisfy
   the auxiliary condition. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static table for prime sieving */
static unsigned char *prime_tbl = NULL;
static idx_t prime_tbl_size = 0;

/* Ensure that prime_tbl has entries for all numbers up to limit */
static void
ensure_prime_tbl(idx_t limit)
{
    if (prime_tbl_size >= limit + 1)
        return;                 /* already sufficient */

    /* free old table if any */
    if (prime_tbl != NULL)
        free(prime_tbl);

    prime_tbl_size = limit + 1;
    /* allocate and init to all 1 (prime) */
    prime_tbl = (unsigned char *)malloc(prime_tbl_size * sizeof(unsigned char));
    if (prime_tbl == NULL)
        exit(1);                /* out of memory */

    for (idx_t i = 0; i <= limit; ++i)
        prime_tbl[i] = 1;

    /* 0 and 1 are not prime */
    if (prime_tbl_size > 0) prime_tbl[0] = 0;
    if (prime_tbl_size > 1) prime_tbl[1] = 0;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (prime_tbl[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_tbl[j] = 0;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* behaviour below offset unimportant */

    /* Even number to partition */
    idx_t ev = n * 2;

    /* Prepare prime table up to ev */
    ensure_prime_tbl(ev);

    seq_t count = 0;
    idx_t half = ev >> 1;        /* ev / 2, integer division */

    for (idx_t p = 2; p <= half; ++p)
    {
        if (!prime_tbl[p] || !prime_tbl[ev - p])
            continue;           /* not a Goldbach pair */

        /* Check auxiliary condition: c(p-2) == c(ev-p-2) */
        bool cond = (prime_tbl[p - 2] == prime_tbl[ev - p - 2]);
        if (cond)
            ++count;
    }
    return count;
}
