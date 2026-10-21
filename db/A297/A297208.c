/*
 * A297208  a(0)=0; for n >= 1, a(n) = a(n-1-A023416(n)) + A000120(n).
 * https://oeis.org/A297208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

/* Recurrence:
 * a(0) = 0
 * a(n) = a(n-1 - A023416(n)) + A000120(n)   for n ≥ 1
 * where A000120(n) = number of ones in binary representation of n
 * and A023416(n) = number of zeros in binary representation of n
 * (i.e. bitlen(n) - popcount(n)).
 *
 * The code memoises computed terms in a dynamic array so that calls
 * can come in arbitrary order.  It is fully portable for all
 * supported seq_t/idx_t types.
 */
seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;      /* array of a(k) */
    static size_t  cap   = 0;        /* allocated capacity (in seq_t) */
    static size_t  done  = 0;        /* number of terms already computed */

    /* Helper: grow cache to hold at least (n+1) entries */
    if (cache == NULL) {
        cap   = 16; /* initial capacity, will grow if needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* no error handling – build system guarantees malloc succeeds */
    } else if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Initialise a(0) if this is the first use */
    if (done == 0) {
        cache[0] = 0;
        done     = 1;
    }

    /* Compute any missing values up to n */
    if (done <= n) {
        for (idx_t i = (idx_t)done; i <= n; ++i) {
            /* popcount of i */
            idx_t temp = i;
            idx_t pop  = 0;
            while (temp) {
                pop++;
                temp &= temp - 1;   /* Kernighan's trick */
            }

            /* bit length of i */
            idx_t bitlen = 0;
            temp = i;
            while (temp) {
                bitlen++;
                temp >>= 1;
            }

            /* zeros in binary representation */
            idx_t zeros = bitlen - pop;

            /* reference index for the recurrence */
            idx_t j = i - 1 - zeros;   /* guaranteed non‑negative */

            /* recurrence: a(i) = a(j) + pop */
            cache[(size_t)i] = cache[(size_t)j] + (seq_t)pop;
        }
        done = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
