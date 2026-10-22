/*
 * A382229  a(0) = 1; thereafter a(n) is the next larger number that compared
 *          to the previous term differs by +-1 in the number of prime factors
 *          counted with multiplicity.
 * https://oeis.org/A382229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A071192(a(n-1)). a(0)=1; thereafter a(n) is the least
 *            integer k > a(n-1) such that |bigomega(k) - bigomega(a(n-1))| =
 *            1
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of already computed terms   */
    static size_t cap   = 0;      /* current allocated capacity        */
    static size_t len   = 0;      /* number of terms stored in cache   */

    /* Grow cache so that it contains at least n+1 terms */
    while (len <= n) {
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        seq_t next;
        if (len == 0) {
            next = (seq_t)1;   /* a(0) = 1 */
        } else {
            seq_t prev = cache[len - 1];
            seq_t cand = prev + (seq_t)1;

            seq_t om_prev = bigomega((idx_t)prev);
            while (1) {
                seq_t om_cand = bigomega((idx_t)cand);
                seq_t diff = (om_cand > om_prev) ? (om_cand - om_prev) : (om_prev - om_cand);
                if (diff == (seq_t)1) break;
                cand = cand + (seq_t)1;
            }
            next = cand;
        }

        cache[len++] = next;
    }

    return cache[n];
}
