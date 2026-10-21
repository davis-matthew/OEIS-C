/*
 * A279431  Numbers k such that k^2 has an odd number of digits in base 2 and
 *          the middle digit is 1.
 * https://oeis.org/A279431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A279431: numbers k such that k^2 has an odd number of binary digits
   and the middle digit is 1.  a(n) is the n‑th such integer (1‑based).  The
   function searches sequentially for qualifying k, caching results so
   that calls may arrive in any order. */
#include <stdlib.h>

/* bit length of a seq_t value.  Assumes seq_t supports >> and comparison to 0. */
static unsigned bitlen_seq_t_local(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Return the n‑th term of the sequence.  n >= 1. */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;     /* cached values of a(i)           */
    static size_t cache_len  = 0;        /* number of values cached          */
    static size_t cache_cap  = 0;        /* allocated capacity of cache      */
    static idx_t next_k      = 1;        /* next integer to test             */

    /* Generate values until we have at least n cached. */
    while ((idx_t)cache_len < n) {
        seq_t k  = (seq_t)next_k;
        seq_t k2 = k * k;                  /* k squared                        */

        unsigned len = bitlen_seq_t_local(k2);
        if (len & 1) {                     /* odd number of bits?              */
            unsigned mid = len >> 1;
            seq_t mask = ((seq_t)1 << mid); /* mask for the middle bit          */
            if (k2 & mask) {                /* middle bit is 1?                 */
                if (cache_len == cache_cap) {   /* grow cache if needed * */
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
            }
        }
        ++next_k;                          /* advance to next integer          */
    }

    return cache[n - 1];                    /* a(1) is at index 0               */
}
