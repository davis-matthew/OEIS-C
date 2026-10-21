/*
 * A248595  Smallest prime number Q such that there is a prime number R such
 *          that floor(Q/R)=prime(n).
 * https://oeis.org/A248595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 580, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search:  a(n) = smallest prime Q such that ∃ prime R with floor(Q/R)=prime(n) */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

/* Return the n-th term of OEIS A248595.  */
seq_t a(idx_t n)
{
    if (n == 0)               /* the offset is 1; n=0 is undefined */
        return 0;

    /* prime(n) --------------------------------------------------------- */
    uint32_t p32 = get_prime(n);          /* 1‑based nth prime, fits in 32‑bit */
    uint64_t  p  = (uint64_t)p32;          /* current prime value          */
    uint64_t  next_p = p + 1;              /* (prime(n) + 1)                */

    /* search over primes R --------------------------------------------- */
    idx_t r_idx = 1;                       /* start with R = prime(1) = 2   */
    while (1) {
        uint64_t R = (uint64_t)get_prime(r_idx);

        /* the interval [R*p , R*(p+1)-1] that must contain the desired Q */
        uint64_t start = R * p;
        uint64_t end   = R * next_p - 1;

        /* smallest prime ≥ start */
        uint64_t Q = next_prime(start);

        if (Q <= end)   /* a valid Q has been found */
            return (seq_t)Q;

        /* otherwise try the next prime R */
        ++r_idx;
    }
}
