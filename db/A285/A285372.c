/*
 * A285372  Sum of the entries in the tenth blocks of all set partitions of
 *          [n].
 * https://oeis.org/A285372
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A285362(n,10) – sum of the entries in the 10th block of
 *            all set partitions of [n]
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Allocation helpers that work in C and C++.  */
#ifdef __cplusplus
#  define ALLOC_SEQ_T(count) new seq_t[(count)]()
#  define FREE_SEQ_T(ptr)    delete[] (ptr)
#else
#  define ALLOC_SEQ_T(count) ((seq_t*)calloc((size_t)(count), sizeof(seq_t)))
#  define FREE_SEQ_T(ptr)    free((void*)(ptr))
#endif

seq_t a(idx_t n) {
    const idx_t K = 10;                    /* 10th block */
    if (n == 0) return (seq_t)0;

    /* Current Stirling numbers S_cur[m] and
       current 10th‑block sums A_cur[m] for partitions of size cur.  */
    seq_t *S_cur = ALLOC_SEQ_T((size_t)n + 1);
    seq_t *A_cur = ALLOC_SEQ_T((size_t)n + 1);
    if (!S_cur || !A_cur) {
        FREE_SEQ_T(S_cur); FREE_SEQ_T(A_cur);
        return (seq_t)0;
    }

    S_cur[0] = 1;                          /* S(0,0)=1, others 0 */

    for (idx_t cur = 0; cur < n; ++cur) {
        idx_t new_elem = cur + 1;           /* the element being added */
        seq_t new_elem_seq = (seq_t)new_elem;

        /* Next arrays for size cur+1 partitions.  */
        seq_t *S_next = ALLOC_SEQ_T((size_t)cur + 2);
        seq_t *A_next = ALLOC_SEQ_T((size_t)cur + 2);
        if (!S_next || !A_next) {
            FREE_SEQ_T(S_cur); FREE_SEQ_T(A_cur);
            FREE_SEQ_T(S_next); FREE_SEQ_T(A_next);
            return (seq_t)0;
        }

        idx_t next_size = cur + 1;          /* number of blocks possible: 0..cur+1 */

        for (idx_t m = 0; m <= next_size; ++m) {
            /* Stirling numbers: S(n+1, m) = m*S(n,m) + S(n,m-1) */
            seq_t s_m   = (m <= cur) ? S_cur[m] : (seq_t)0;
            seq_t s_m1  = (m > 0)   ? S_cur[m-1] : (seq_t)0;
            S_next[m] = (seq_t)m * s_m + s_m1;

            /* 10th‑block sums: */
            seq_t a_m   = (m <= cur) ? A_cur[m] : (seq_t)0;

            /* Join an existing block (m choices). */
            seq_t join_contrib = a_m * (seq_t)m + new_elem_seq * s_m;

            /* Add a new block created from a partition with m‑1 blocks. */
            seq_t addnew = 0;
            if (m > 0) {
                idx_t prev_blocks = m - 1;
                seq_t s_prev = S_cur[prev_blocks];
                seq_t a_prev = A_cur[prev_blocks];
                if (prev_blocks >= K) {
                    /* New block is after the 10th block: 10th is unchanged. */
                    addnew = a_prev;
                } else if (m == K) {
                    /* New block becomes the 10th block. */
                    addnew = new_elem_seq * s_prev;
                }
            }

            A_next[m] = join_contrib + addnew;
        }

        /* Prepare for next iteration. */
        FREE_SEQ_T(S_cur); FREE_SEQ_T(A_cur);
        S_cur = S_next; A_cur = A_next;
    }

    /* Sum over all partitions that have at least 10 blocks. */
    seq_t result = 0;
    for (idx_t m = K; m <= n; ++m) result += A_cur[m];

    FREE_SEQ_T(S_cur); FREE_SEQ_T(A_cur);
    return result;
}
