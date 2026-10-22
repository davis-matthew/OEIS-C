/*
 * A347744  Number of compositions (ordered partitions) of n into at most 2
 *          prime parts (counting 1 as a prime).
 * https://oeis.org/A347744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a prime (including 1) else 0 * + the number of
 *            ordered pairs of primes (p,q) with p+q = n * (primes are the
 *            positive integers p >= 1 that are not divisible by any smaller *
 *            number other than 1; 1 is considered prime for this sequence)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc / realloc / free */

static idx_t cur_max = 0;          /* maximum index currently sieved */
static unsigned char *prime_flags = NULL;  /* prime_flags[i] == 1 iff i is prime (1 considered prime) */

/* Ensure that prime_flags[0 .. limit] is correct.  Expands the sieve
 * only when necessary.  The function is re‑entrant and works for both
 * C and C++ builds. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= cur_max)
        return;                      /* already sieved up to this limit */

    /* enlarge the array to hold indices up to 'limit' (inclusive) */
    size_t new_size = (size_t)(limit + 1);
    unsigned char *new_flags = (unsigned char *)realloc(prime_flags, new_size);
    if (!new_flags)
        abort();                      /* allocation failure */
    prime_flags = new_flags;

    /* initialise all entries as potentially prime */
    for (size_t i = 0; i < new_size; ++i)
        prime_flags[i] = 1;
    prime_flags[0] = 0;                 /* 0 is not prime */
    prime_flags[1] = 1;                 /* 1 is treated as prime */

    /* run the classic sieve of Eratosthenes on the newly allocated region */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flags[j] = 0;
        }
    }

    cur_max = limit;                    /* remember the sieved bound */
}

seq_t a(idx_t n)
{
    if (n == 0)            /* by definition a(0) = 1 (empty composition) */
        return (seq_t)1;

    ensure_sieve(n);              /* guarantee prime_flags correctly filled up to n */

    /* count the single‑part composition if n itself is prime */
    seq_t res = (prime_flags[n] ? (seq_t)1 : (seq_t)0);

    /* count all ordered pairs of primes (p,q) with p+q == n */
    for (idx_t p = 1; p <= n - 1; ++p) {
        if (prime_flags[p] && prime_flags[n - p])
            res += (seq_t)1;
    }

    return res;
}
