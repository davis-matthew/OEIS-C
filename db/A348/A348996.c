/*
 * A348996  a(n) = usigma(A276086(n)), where usigma (A034448) is
 *          multiplicative with a(p^e) = (p^e)+1, and A276086 gives the prime
 *          product form of primorial base expansion of n.
 * https://oeis.org/A348996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A034448(A276086(n))
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A34: usigma, A276086: prime product form of primorial base expansion of n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t); */

static uint64_t next_prime(uint64_t n)
{
    if (n <= 2ULL) return 2ULL;
    if ((n & 1ULL) == 0) ++n;
    while (!is_prime((idx_t)n))
        n += 2;
    return n;
}

seq_t a(idx_t n)
{
    /* 0 gives the empty product */
    if (n == 0) return (seq_t)1;

    /* build list of primes and their primorials up to the point where
       primorial > n.  primorial[0] = 1 (empty product). */
    size_t  pcnt = 0;
    uint64_t prime = 2;
    uint64_t prim  = 1;                /* current primorial */

    uint64_t *pr = NULL;               /* primes[0..pcnt-1]   */
    uint64_t *pml = NULL;              /* primorials[0..pcnt-1] */

    while (prim <= (uint64_t)n) {
        pr   = (uint64_t*)realloc(pr,   (pcnt+1)*sizeof(uint64_t));
        pml  = (uint64_t*)realloc(pml,  (pcnt+1)*sizeof(uint64_t));
        if (!pr || !pml) return (seq_t)0;   /* allocation failure */

        pr[pcnt]   = prime;
        pml[pcnt]  = prim;
        pcnt++;

        /* advance */
        if (prime > UINT64_MAX / prim) break;
        prim  = prim * prime;
        prime = next_prime(prime + 1);
    }

    /* traverse digits from most significant to least */
    uint64_t  remain = (uint64_t)n;
    seq_t     result = (seq_t)1;

    for (size_t i = pcnt; i-- > 0; ) {
        uint64_t p   = pr[i];
        uint64_t base = pml[i];
        uint64_t digit = remain / base;
        remain = remain % base;

        if (digit == 0) continue;

        seq_t pow = (seq_t)1;
        for (uint64_t t = 0; t < digit; ++t)
            pow *= (seq_t)p;

        seq_t factor = pow + (seq_t)1;
        result *= factor;
    }

    free(pr);
    free(pml);
    return result;
}
