/*
 * A365886  Numbers k whose least prime divisor is smaller than its exponent
 *          in the prime factorization of k.
 * https://oeis.org/A365886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 8597, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute numbers k whose least prime divisor is smaller than its exponent
   in the prime factorization of k.  */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms = NULL;          /* array of terms */
    static idx_t   cap    = 0;           /* current capacity of array */
    static idx_t   count  = 0;           /* number of terms stored */
    static uint64_t last = 1;            /* last integer examined */

    if (n == 0) return 0;                /* safety – not used */

    /* if we already have enough terms, return the requested one */
    if (n <= count) return terms[n - 1];

    /* initialise storage if necessary */
    if (cap == 0) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we reach the requested index */
    while (count < n) {
        ++last;                           /* start at 2, then 3, ... */
        if (last < 2) continue;

        uint64_t k = last;                /* candidate integer */
        uint64_t p = 0;                   /* least prime divisor (0 if none found) */

        /* find least prime divisor of k */
        for (uint64_t d = 2; d * d <= k; ++d) {
            if (k % d == 0) {
                p = d;
                break;
            }
        }
        if (p == 0) continue;             /* k is prime → exponent 1 → skip */

        /* compute exponent of the least prime divisor */
        uint64_t exp = 0;
        uint64_t t = k;
        while (t % p == 0) {
            ++exp;
            t /= p;
        }

        if (p < exp) {                    /* condition satisfied */
            if (count == cap) {           /* grow array if needed */
                cap <<= 1;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[count++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
