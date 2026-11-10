/*
 * A302441  a(n) is the length of the n-th run of equal terms in A303579.
 * https://oeis.org/A302441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run of equal terms in A303579;
 *            computed by generating A303579 (lengths of nondecreasing runs of
 *            phi(k)) and grouping equal terms
 * Range:     u8 n <= 7, u16 n <= 2610, u32 n <= 26224, u64 n <= 26224,
 *            u128 n <= 26224, big any n
 * Validated: standard, pass, 94 terms through n = 9904 (2026-11-08)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static idx_t cur_k = 2;          /* next k whose phi is evaluated */
    static seq_t prev_phi = 1;       /* phi(cur_k - 1) */
    static idx_t nd_len = 1;         /* length of current nondecreasing run */
    static bool has_last = false;    /* have a previous A303579 term */
    static idx_t last_val = 0;       /* last A303579 term */
    static idx_t last_cnt = 0;       /* its repeat count */
    static idx_t used = 0;           /* finished equal-runs stored */
    static size_t cap = 0;
    static seq_t *runs = NULL;
    static bool init = false;

    if (!init) {
        prev_phi = totient(1);
        init = true;
    }

    while (used < n) {
        seq_t ph = totient(cur_k);
        if (ph < prev_phi) {
            /* A303579 term emitted: nd_len */
            idx_t v = nd_len;
            nd_len = 1;
            if (has_last && v == last_val) {
                ++last_cnt;
            } else {
                if (has_last) {
                    if ((size_t)used >= cap) {
                        size_t nc = cap ? cap << 1 : 64;
                        seq_t *tmp = mem_resize(seq_t, runs, nc);
                        if (!tmp) return 0;
                        runs = tmp;
                        cap = nc;
                    }
                    runs[used++] = (seq_t)last_cnt;
                }
                has_last = true;
                last_val = v;
                last_cnt = 1;
            }
        } else {
            ++nd_len;
        }
        prev_phi = ph;
        ++cur_k;
    }
    return runs[n - 1];
}
