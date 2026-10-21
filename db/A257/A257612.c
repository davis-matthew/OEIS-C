/*
 * A257612  Triangle read by rows: T(n,k) = t(n-k, k); t(n,m) = f(m)*t(n-1,m)
 *          + f(n)*t(n,m-1), where f(x) = 4*x + 2.
 * https://oeis.org/A257612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle read by rows generated
 *            by T(0,0) = 1 T(n, k) = (4*k + 2) * T(n-1, k) + (4*(n-k) + 2) *
 *            T(n-1, k-1) for n ≥ 1, 0 ≤ k ≤ n The sequence a(n) lists the
 *            terms of T in row‑major order: a(0)=T(0,0), a(1)=T(1,0),
 *            a(2)=T(1,1), a(3)=T(2,0), …
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 47, u64 n <= 124,
 *            u128 n <= 334, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <new>   /* for operator new[] and delete[] */
#endif

/* Resize an array of seq_t, preserving existing elements.
   Works for both C and C++. */
static seq_t *realloc_seq_t(seq_t *old, size_t old_cap, size_t new_cap)
{
#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        newp[i] = old[i];
    delete[] old;
    return newp;
#else
    void *tmp = realloc(old, new_cap * sizeof(seq_t));
    return (seq_t *)tmp;
#endif
}

/* Return the nth term of sequence A257612. */
seq_t a(idx_t n)
{
    /* Static state kept between calls. */
    static seq_t *seq   = NULL;          /* flattened triangle terms          */
    static size_t seq_cap  = 0;          /* allocated capacity of seq         */
    static size_t seq_len  = 0;          /* number of terms stored            */

    static seq_t *prev   = NULL;         /* previous row                       */
    static size_t prev_len  = 0;         /* length of previous row             */

    static seq_t *cur    = NULL;         /* buffer for current row             */
    static size_t cur_cap  = 0;          /* capacity of cur buffer             */

    static idx_t next_row = 0;           /* next row index to compute          */

    const seq_t A = (seq_t)4;
    const seq_t B = (seq_t)2;

    /* Compute rows until we have at least (n+1) terms. */
    while (seq_len <= n) {
        idx_t r = next_row;            /* current row number */
        size_t new_len = (size_t)r + 1;

        /* Ensure cur has enough space. */
        if (cur_cap < new_len)
            cur = realloc_seq_t(cur, cur_cap, new_len);

        if (r == 0) {
            cur[0] = (seq_t)1;
        } else {
            /* T(r,0) = (4*0+2) * T(r-1,0) */
            cur[0] = B * prev[0];

            for (size_t k = 1; k <= r; ++k) {
                seq_t term1 = 0;
                if (k <= prev_len - 1) {
                    seq_t fk = A * (seq_t)k + B;
                    term1 = fk * prev[k];
                }
                seq_t fk2 = A * (seq_t)(r - k) + B;
                seq_t term2 = fk2 * prev[k - 1];
                cur[k] = term1 + term2;
            }
        }

        /* Append the new row to the flattened sequence. */
        size_t needed_len = seq_len + new_len;
        if (needed_len > seq_cap)
            seq = realloc_seq_t(seq, seq_cap, needed_len);

        for (size_t i = 0; i < new_len; ++i)
            seq[seq_len++] = cur[i];

        /* Prepare for next iteration: swap prev and cur. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_len = new_len;

        ++next_row;
    }

    return seq[(size_t)n];
}
