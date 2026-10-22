/*
 * A358349  A puzzle array read by antidiagonals.
 * https://oeis.org/A358349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = sequence defined by the puzzle array. Recurrence: T(1,m)
 *            = m, T(n,1) = 1, T(n,m) = T(n-1,m-1) * T(n-1,m) + T(n,m-1). a(n)
 *            is read by antidiagonals: a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1),
 *            etc
 * Range:     u8 n <= 23, u16 n <= 31, u32 n <= 49, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state: cached sequence and last computed diagonal */
    static seq_t *seq_cache = NULL;
    static size_t seq_len = 0, seq_cap = 0;

    static seq_t *diag_prev  = NULL; /* diagonal d-1 */
    static size_t  diag_prev_len = 0;
    static seq_t *diag_prev2 = NULL; /* diagonal d-2 */
    static size_t  diag_prev2_len = 0;
    static size_t  cur_d = 0;        /* number of diagonals processed */

    /* compute diagonals until we have at least n terms */
    while (cur_d * (cur_d + 1) / 2 < n)
    {
        size_t d = cur_d + 1;                       /* current diagonal number */
        seq_t *diag_cur = (seq_t *)malloc(d * sizeof(seq_t));
        if (!diag_cur) return 0; /* allocation failure, value undefined */

        for (size_t i = 1; i <= d; ++i)
        {
            size_t j = d + 1 - i;

            if (i == 1)
            {
                /* T(1,j) = j */
                diag_cur[i - 1] = (seq_t)j;
            }
            else if (j == 1)
            {
                /* T(i,1) = 1 */
                diag_cur[i - 1] = 1;
            }
            else
            {
                /* T(i,j) uses previous two diagonals */
                seq_t a = diag_prev2[i - 2];           /* T(i-1,j-1) */
                seq_t b = diag_prev[i - 2];            /* T(i-1,j)   */
                seq_t c = diag_prev[i - 1];            /* T(i,j-1)   */
                diag_cur[i - 1] = a * b + c;
            }
        }

        /* append current diagonal to sequence cache */
        size_t new_len = seq_len + d;
        if (new_len > seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap * 2 : 256;
            while (new_cap < new_len) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(diag_cur); return 0; }
            seq_cache = tmp;
            seq_cap = new_cap;
        }
        for (size_t i = 0; i < d; ++i) seq_cache[seq_len + i] = diag_cur[i];
        seq_len = new_len;

        /* rotate diagonal buffers */
        free(diag_prev2);
        diag_prev2 = diag_prev;
        diag_prev2_len = diag_prev_len;
        diag_prev = diag_cur;
        diag_prev_len = d;
        ++cur_d;
    }

    return seq_cache[n - 1];
}
