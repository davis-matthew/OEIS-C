/*
 * A389412  a(n) is the largest k such that (A005117(n) - 1) + A005117(i) is
 *          squarefree for 1 <= i <= k.
 * https://oeis.org/A389412
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest k such that (A005117(n)-1)+A005117(i) is
 *            squarefree for 1<=i<=k. Implementation: generate squarefree
 *            numbers on demand and test each sum for squarefreeness
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_squarefree.h"

static seq_t *sq = NULL;        /* array of generated squarefree numbers */
static size_t sq_cap = 0;       /* allocated capacity of the array */
static idx_t sq_cnt = 0;        /* number of squarefree numbers stored */
static seq_t last_candidate = 1;/* last integer tested for squarefreeness */

/* Ensure that at least 'need' squarefree numbers are stored (indices 0..need-1). */
static void ensure_squarefree_at(idx_t need)
{
    /* Add the first squarefree number 1 if not yet present. */
    if (sq_cnt == 0) {
        if (sq_cap == 0) {
            sq_cap = 16;
            sq = (seq_t *)malloc(sq_cap * sizeof(seq_t));
        }
        sq[0] = 1;
        sq_cnt = 1;
        last_candidate = 1;
    }

    while (sq_cnt < need) {
        seq_t cand = last_candidate + 1;
        last_candidate = cand;
        if (is_squarefree(cand)) {
            if (sq_cnt >= sq_cap) {
                size_t new_cap = sq_cap << 1;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                memcpy(tmp, sq, sq_cnt * sizeof(seq_t));
                free(sq);
                sq = tmp;
                sq_cap = new_cap;
            }
            sq[sq_cnt++] = cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* the offset is 2; values below are unspecified */

    /* Make sure we have at least the n-th squarefree number. */
    ensure_squarefree_at(n);
    seq_t d = sq[n - 1] - 1;  /* m_n - 1 */

    size_t k = 0;
    for (idx_t i = 1; ; ++i) {
        ensure_squarefree_at(i);          /* ensure A005117(i) exists */
        seq_t s = d + sq[i - 1];          /* (m_n-1) + A005117(i) */
        if (is_squarefree(s))
            ++k;
        else
            break;
    }
    return (seq_t)k;
}
