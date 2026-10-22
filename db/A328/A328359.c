/*
 * A328359  Numbers k such that Omega(k - 2) = Omega(k) = Omega(k + 2) where
 *          Omega = A001222.
 * https://oeis.org/A328359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A328359
   a(n) = the nth integer k (k ≥ 5) such that
          Omega(k-2) = Omega(k) = Omega(k+2)
   (search approach) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *store = NULL;
    static size_t stored = 0;   /* number of terms cached                 */
    static size_t cap = 0;      /* allocated capacity of the cache         */
    static idx_t last_checked = 0;  /* largest number examined so far       */

    if (n == 0) return 0;           /* offset begins at 1                    */

    size_t target = (size_t)n;      /* how many terms we need                 */

    if (stored < target) {
        /* start from the next integer after the last checked one,
           but at least 5, the first possible term               */
        idx_t cand = (last_checked == 0 ? 5 : last_checked + 1);

        while (stored < target) {
            seq_t om1 = bigomega(cand - 2);
            seq_t om2 = bigomega(cand);
            seq_t om3 = bigomega(cand + 2);

            if (om1 == om2 && om2 == om3) {
                /* grow cache if necessary */
                if (stored == cap) {
                    size_t new_cap = (cap == 0) ? 8 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(store,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();     /* allocation failure */
                    store = tmp;
                    cap = new_cap;
                }
                store[stored++] = (seq_t)cand;
            }
            cand++;
        }
        last_checked = cand - 1;
    }

    return store[n - 1];
}
