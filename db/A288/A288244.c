/*
 * A288244  Numbers k such that prime(k)*prime(k+1)*prime(k+2) mod prime(k+3)
 *          is even.
 * https://oeis.org/A288244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A288244: a(n) returns the n-th integer k such that
 * prime(k)*prime(k+1)*prime(k+2) mod prime(k+3) is even.
 * Indexing starts at 1 (a(1) = 1).  The result is stored in
 * a static cache so that calls are independent of the order
 * of the queries.  This uses the supplied get_prime() helper
 * to obtain kth primes and mul_mod64() for safe modulo
 * multiplication on 64‑bit values. */
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/mul_mod64.h"

seq_t a(idx_t n)
{
    /* handle trivial offset < 1 (not used but safe) */
    if (n < 1)
        return (seq_t)0;

    /* static cache of computed sequence terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  len    = 0;
    static idx_t   last_k = 0;      /* last k that was checked */

    /* ensure cache has enough capacity */
    if (len + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < len + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                /* allocation failure, abort */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* extend the cache until the requested term exists */
    while (len < (size_t)n) {
        ++last_k;  /* next k to test */

        /* fetch the four consecutive primes */
        uint32_t p0 = get_prime(last_k);     /* prime(k)   */
        uint32_t p1 = get_prime(last_k + 1); /* prime(k+1) */
        uint32_t p2 = get_prime(last_k + 2); /* prime(k+2) */
        uint32_t p3 = get_prime(last_k + 3); /* prime(k+3) */

        /* compute product modulo p3, carefully avoiding overflow */
        uint64_t mod  = (uint64_t)p3;
        uint64_t prod = mul_mod64((uint64_t)p0, (uint64_t)p1, mod);
        prod = mul_mod64(prod, (uint64_t)p2, mod);

        /* check if the residue is even */
        if ((prod & 1U) == 0U) {
            cache[len++] = (seq_t)last_k;   /* store the qualifying k */
        }
    }

    return cache[n - 1];
}
