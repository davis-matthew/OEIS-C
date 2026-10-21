/*
 * A257128  a(n) = the smallest number k such that A227362(k) = A009995(n).
 * https://oeis.org/A257128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating all non‑empty subsets of the digits 0–9.
   For each subset S, compute v = reverse concatenation of distinct digits
   of S, i.e. the digits of S sorted in descending order.
   The minimal integer k having exactly the same distinct digits is
   the digits of S sorted in ascending order, with the smallest non‑zero
   digit (if any) moved to the front.  All these pairs (v, k) are stored,
   sorted by v, and a(n) is the k corresponding to the n‑th smallest v. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"

struct term { seq_t v, k; };

static int cmp_terms(const void *a, const void *b)
{
    const struct term *ta = (const struct term *)a;
    const struct term *tb = (const struct term *)b;
    return cmp_seq_t(&ta->v, &tb->v);
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static struct term terms[1023];
    if (!initialized) {
        size_t idx = 0;
        /* Enumerate all non‑empty subsets of {0,…,9} */
        for (int mask = 1; mask < 1024; ++mask) {
            /* Build v: digits of mask sorted descending */
            seq_t v = 0;
            for (int d = 9; d >= 0; --d) {
                if (mask & (1 << d)) {
                    v = v * 10 + (seq_t)d;
                }
            }
            /* Gather digits of the subset in ascending order */
            unsigned char digs[10];
            int cnt = 0;
            for (int d = 0; d <= 9; ++d) {
                if (mask & (1 << d)) {
                    digs[cnt++] = (unsigned char)d;
                }
            }
            /* Compute minimal k */
            seq_t k;
            if (cnt == 1) {
                k = digs[0];
            } else {
                /* If zero is present, move the smallest non‑zero digit to front */
                if (digs[0] == 0) {
                    int j = 1;
                    while (j < cnt && digs[j] == 0) j++;
                    if (j < cnt) {
                        unsigned char tmp = digs[0];
                        digs[0] = digs[j];
                        digs[j] = tmp;
                    }
                }
                seq_t tmpk = 0;
                for (int i = 0; i < cnt; ++i) {
                    tmpk = tmpk * 10 + (seq_t)digs[i];
                }
                k = tmpk;
            }
            terms[idx].v = v;
            terms[idx].k = k;
            ++idx;
        }
        /* Sort by v */
        qsort(terms, 1023, sizeof terms[0], cmp_terms);
        initialized = true;
    }
    if (n == 0 || n > 1023) return 0;
    return terms[n-1].k;
}
