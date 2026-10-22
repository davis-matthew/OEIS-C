/*
 * A373497  If n is prime, a(n) = 1 if the number of steps in its prime index
 *          chain is odd, a(n) = -1 if the number of steps is even, and a(n) =
 *          0 is n is composite or 1.
 * https://oeis.org/A373497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373497
    a(n) =  1  if n is prime and the prime‑index chain length is odd
    a(n) = -1  if n is prime and the prime‑index chain length is even
    a(n) =  0  otherwise (n composite or 1)                         */

#include <stdint.h>
#include <stdlib.h>

/* static state shared between calls */
static idx_t   currentMax  = 0;          /* maximum n that has been sieved */
static idx_t*  primeIndex  = NULL;      /* prime_index[p] = k if p is the k‑th prime, else 0 */
static unsigned char* composite = NULL;/* sieve flag: 1 = composite, 0 = prime or 0/1 */

static void ensure_sieve(idx_t n)
{
    if (n <= currentMax) return;           /* nothing to do */

    /* free previous tables */
    if (composite) free(composite);
    if (primeIndex) free(primeIndex);

    /* allocate new tables (C++ requires explicit casts for void*) */
    composite = (unsigned char*)calloc((size_t)(n + 1), 1);
    primeIndex = (idx_t*)calloc((size_t)(n + 1), sizeof(idx_t));
    if (!composite || !primeIndex) {
        /* fatal error – out of memory */
        exit(1);
    }

    /* sieve of Eratosthenes */
    idx_t primeCnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!composite[i]) {                    /* i is prime */
            ++primeCnt;
            primeIndex[i] = primeCnt;           /* store its index (1‑based) */
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i) {
                    composite[j] = 1;           /* mark composite */
                }
            }
        }
    }

    currentMax = n;
}

seq_t a(idx_t n)
{
    if (n <= 1)                      /* a(1) = 0 by definition */
        return 0;

    ensure_sieve(n);                 /* make sure we know primes up to n */

    if (!primeIndex[n])              /* n is composite or 1 */
        return 0;

    /* follow the prime‑index chain */
    idx_t steps = 0;
    idx_t cur = n;
    while (primeIndex[cur]) {        /* cur is prime */
        cur = primeIndex[cur];       /* replace by its prime index */
        ++steps;
    }

    /* steps%2 == 1 -> odd -> 1 ; even -> -1 */
    return (steps & 1) ? (seq_t)1 : (seq_t)-1;
}
