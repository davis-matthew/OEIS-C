/*
 * A352774  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number that has not appeared that does not share a factor with
 *          a(n-2) + a(n-1) or a(n-2)*a(n-1).
 * https://oeis.org/A352774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest unused positive integer that shares no
 *            factor with (a(n-2)+a(n-1)) or (a(n-2)*a(n-1)). a(1)=1, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation memoises terms and a used‑number bitmap to allow
   any order of calls.  All arithmetic is done in seq_t.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *seq     = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;

    static unsigned char *used     = NULL;
    static idx_t used_cap          = 0;

    /* initialise */
    if (seq_len == 0) {
        /* allocate sequence array: cap 4, positions 1 and 2 used */
        seq_cap = 4;
        seq = (seq_t*)malloc(sizeof(seq_t) * seq_cap);
        seq[1] = 1;
        seq[2] = 2;
        seq_len = 2;

        /* allocate bitmap for used numbers: initial cap 64 */
        used_cap = 64;
        used = (unsigned char*)calloc((size_t)used_cap + 1, sizeof(unsigned char));
        used[1] = 1;
        used[2] = 1;
    }

    if (n <= seq_len)
        return seq[(size_t)n];

    /* grow sequence until n */
    for (idx_t cur = seq_len + 1; cur <= n; ++cur) {
        /* ensure sequence array capacity */
        if (seq_cap < (size_t)(cur + 1)) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 4;
            while (new_cap < (size_t)(cur + 1))
                new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            seq = tmp;
            seq_cap = new_cap;
        }

        seq_t sum = seq[(size_t)(cur-2)] + seq[(size_t)(cur-1)];
        seq_t prod = seq[(size_t)(cur-2)] * seq[(size_t)(cur-1)];

        seq_t cand = 1;
        for (;;) {
            /* ensure used bitmap capacity */
            idx_t cand_idx = (idx_t)cand;
            if (cand_idx > used_cap) {
                idx_t new_cap = used_cap ? used_cap * 2 : 64;
                if (new_cap < cand_idx * 2) new_cap = cand_idx * 2;
                unsigned char *tmp = (unsigned char*)realloc(used,
                                                             (size_t)new_cap + 1);
                if (!tmp) exit(EXIT_FAILURE);
                /* initialise new entries to 0 */
                for (idx_t i = used_cap + 1; i <= new_cap; ++i)
                    tmp[i] = 0;
                used = tmp;
                used_cap = new_cap;
            }

            if (used[(size_t)cand_idx]) {
                ++cand;
                continue;
            }
            if (gcd(cand, sum) != 1) {
                ++cand;
                continue;
            }
            if (gcd(cand, prod) != 1) {
                ++cand;
                continue;
            }
            break;            /* candidate accepted */
        }

        seq[(size_t)cur] = cand;
        used[(size_t)cand] = 1;
        seq_len = (size_t)cur;
    }

    return seq[(size_t)n];
}
