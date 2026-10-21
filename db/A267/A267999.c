/*
 * A267999  Numbers n > 1 such that gcd(n, 2^n - 2) = 1.
 * https://oeis.org/A267999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A267999 – numbers n > 1 such that gcd(n, 2^n‑2)=1 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /*  Memoized one‑based sequence */
    static seq_t *seq   = NULL;   /* array of found numbers          */
    static size_t cap   = 0;      /* capacity of seq (in elements)   */
    static size_t sz    = 0;      /* number of elements already stored */
    static idx_t  cand  = 3;      /* next odd integer to test         */

    /* grow the array when necessary */
    if (sz > cap) {
        /* should never happen; defensive copy */
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        seq_t *tmp = seq_realloc(seq, cap, new_cap);
        if (!tmp) abort();
        seq = tmp;
        cap = new_cap;
    }

    /* build sequence up to requested index n (1‑based) */
    while (sz < n) {
        /* skip even values (should not happen after initialization) */
        if ((cand & 1ULL) == 0) { cand++; continue; }

        /* candidate is >1 by construction */
        seq_t k   = (seq_t)cand;
        seq_t p   = mod_pow((seq_t)2, cand, k);   /* 2^cand mod k */
        seq_t r;
        if (p >= (seq_t)2) r = p - (seq_t)2;
        else               r = p + k - (seq_t)2;  /* make r in [0,k-1] */

        if (gcd(k, r) == (seq_t)1) {
            /* store valid term */
            if (sz == cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = seq_realloc(seq, cap, new_cap);
                if (!tmp) abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[sz++] = k;
        }
        cand++;   /* advance to next integer */
    }

    return seq[n - 1];   /* 1‑based indexing */
}
