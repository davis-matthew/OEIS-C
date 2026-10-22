/*
 * A372042  Monogamously Faithful Primes (primes that are sexy primes with
 *          only one other prime in their pair).
 * https://oeis.org/A372042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the monogamously faithful prime sequence A372042
   by sieving up to a fixed limit and collecting all primes p
   such that exactly one of p±6 is prime, and that partner
   also has exactly one such partner.  The sequence is stored
   in a static array for fast repeated queries. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* External types supplied by the build environment */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static variables initialised on first call */
    static uint8_t *isPrime   = NULL;   /* isPrime[i] == 1 iff i is prime */
    static seq_t  *seq        = NULL;   /* sequence elements */
    static size_t seqSize     = 0;      /* number of elements stored */
    static size_t seqCap      = 0;      /* current capacity of seq */

    /* A limit that comfortably exceeds the 10 000th term.
       25 000 000 primes produce well more than 10 000 monogamous
       faithful primes and fits easily in memory. */
    const size_t LIMIT = 25000000;

    /* Initialise the sieve and sequence on the first call. */
    if (isPrime == NULL)
    {
        /* Allocate and initialise the sieve array. */
        isPrime = (uint8_t *)malloc(LIMIT + 1);
        if (isPrime == NULL)
            return (seq_t)0;                /* out of memory */

        memset(isPrime, 1, LIMIT + 1);
        isPrime[0] = 0;
        isPrime[1] = 0;

        for (size_t i = 2; i * i <= LIMIT; ++i)
            if (isPrime[i])
                for (size_t j = i * i; j <= LIMIT; j += i)
                    isPrime[j] = 0;

        /* Allocate initial capacity for the sequence array. */
        seqCap = 2048;
        seq = (seq_t *)malloc(seqCap * sizeof(seq_t));
        if (seq == NULL) {
            free(isPrime);
            return (seq_t)0;
        }
        seqSize = 0;

        /* Find all monogamously faithful primes up to LIMIT. */
        for (size_t p = 5; p + 6 <= LIMIT; ++p) {
            if (!isPrime[p]) continue;

            bool left  = (p > 6)  && isPrime[p - 6];
            bool right =          isPrime[p + 6];

            /* p must have exactly one sexy partner. */
            if ((left + right) != 1) continue;

            size_t q = left ? p - 6 : p + 6;
            bool q_left  = (q > 6)  && isPrime[q - 6];
            bool q_right =          isPrime[q + 6];

            /* That partner must also have exactly one partner. */
            if ((q_left + q_right) != 1) continue;

            /* Grow the array if needed. */
            if (seqSize == seqCap) {
                size_t newCap = seqCap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, newCap * sizeof(seq_t));
                if (tmp == NULL) {
                    free(isPrime);
                    free(seq);
                    return (seq_t)0;
                }
                seq = tmp;
                seqCap = newCap;
            }
            seq[seqSize++] = (seq_t)p;
        }
    }

    /* Return the requested element if it exists. */
    if (n >= seqSize)
        return (seq_t)0;          /* out of range */
    return seq[n];
}
