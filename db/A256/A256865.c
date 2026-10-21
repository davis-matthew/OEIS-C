/*
 * A256865  Numbers divisible by prime(d+1) for each digit d of their base-5
 *          representation.
 * https://oeis.org/A256865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑5 representation contains digit d
   ⇒ the number is divisible by prime(d+1) (prime(1)=2, prime(2)=3, …). */

/* The function scans non‑negative integers in order, keeps the ones that
   satisfy the property in a static cache, and returns the `n`‑th cached
   value (1‑based).  This works for any seq_t that supports the usual
   arithmetic and comparison operators. */
#include <stdbool.h>
#include <stdlib.h>

/* primes for digits 0‑4: prime(1)=2, prime(2)=3, prime(3)=5,
   prime(4)=7, prime(5)=11 */
static const unsigned primes[5] = {2, 3, 5, 7, 11};

/* Does the integer `x` satisfy the digit‑divisibility rule? */
static inline bool satisfies(seq_t x)
{
    if (x == (seq_t)0)
        return (x % (seq_t)primes[0]) == (seq_t)0;   /* 0 is always divisible by 2 */

    seq_t   m    = x;
    unsigned mask = 0;

    /* Build a bitmask of which digits appear in the base‑5 representation. */
    while (m != (seq_t)0)
    {
        unsigned d = (unsigned)(m % (seq_t)5);     /* current base‑5 digit */
        mask        |= 1u << d;
        m           /= (seq_t)5;
    }

    /* Check all distinct digits that appeared. */
    for (unsigned d = 0; d < 5; ++d)
    {
        if (mask & (1u << d))
            if (x % (seq_t)primes[d] != (seq_t)0)
                return false;
    }
    return true;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cache of found terms          */
    static size_t capacity   = 0;      /* allocated size of cache      */
    static idx_t  count      = 0;      /* number of terms cached so far*/
    static idx_t  nextScan   = 0;      /* next integer to test          */

    /* Grow the cache while it is not yet large enough. */
    while (count < n)
    {
        if (satisfies((seq_t)nextScan))
        {
            if ((size_t)count == capacity)
            {
                size_t newCap = capacity ? capacity * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
                if (!tmp) exit(1);        /* allocation failed */
                cache    = tmp;
                capacity = newCap;
            }
            cache[count++] = (seq_t)nextScan;
        }
        ++nextScan;
    }

    return cache[(size_t)(n - 1)];
}
