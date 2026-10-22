/*
 * A307774  Number of partitions of n into 3 parts such that the middle and
 *          largest parts are both prime.
 * https://oeis.org/A307774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a prime sieve and a double loop to count partitions of n
   into three non‑decreasing parts where the middle and largest
   parts are both prime.  This follows the description of OEIS
   A307774. */

#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve state */
static bool *prime_flag = NULL;
static idx_t sieve_limit = 0;

/* Extend the sieve so that all numbers up to 'limit' are known
   whether prime or not.  The sieve uses a standard Eratosthenes
   algorithm. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate or reallocate the flag array.  In C++ a cast is
       required because calloc returns void*. */
    idx_t new_size = limit + 1;
    bool *new_flag = (bool*)calloc(new_size, sizeof(bool));
    if (!new_flag)
        exit(EXIT_FAILURE);

    /* Assume all numbers ≥ 2 are prime initially. */
    for (idx_t i = 2; i < new_size; ++i)
        new_flag[i] = true;

    /* Eratosthenes sieve. */
    for (idx_t p = 2; p * p <= limit; ++p)
    {
        if (new_flag[p])
        {
            for (idx_t j = p * p; j <= limit; j += p)
                new_flag[j] = false;
        }
    }

    /* Replace the old flag array. */
    if (prime_flag)
        free(prime_flag);
    prime_flag = new_flag;
    sieve_limit = limit;
}

/* Count the partitions as described. */
seq_t a(idx_t n)
{
    if (n < 3)   /* no 3-part partition possible */
        return (seq_t)0;

    /* Ensure primes up to n for checks of middle and largest parts. */
    ensure_prime_sieve(n);

    seq_t count = 0;

    /* Smallest part k ranges from 1 to floor(n/3). */
    for (idx_t k = 1; k <= n / 3; ++k)
    {
        /* Middle part i ranges from k up to floor((n-k)/2)
           to keep i ≤ j. */
        idx_t max_i = (n - k) / 2;
        for (idx_t i = k; i <= max_i; ++i)
        {
            if (!prime_flag[i])           /* middle part must be prime */
                continue;

            idx_t j = n - k - i;          /* largest part */

            if (j < i)                    /* maintain non‑decreasing order */
                continue;

            if (!prime_flag[j])           /* largest part must be prime */
                continue;

            ++count;                      /* valid partition found */
        }
    }

    return count;
}
