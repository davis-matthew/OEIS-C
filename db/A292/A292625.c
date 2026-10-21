/*
 * A292625  Triangle read by rows: row n gives y transposed, where y is the
 *          solution to the matrix equation M*y=b, where the matrix M and
 *          vector b are defined by M(i,j) = ((2^(i+1) + 1)^(j-1) + 1)/2 and
 *          b(i) = ((2^(i+1)+1)^n + 1)/2 for 1 <= i,j <= n.
 * https://oeis.org/A292625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 21, i64 n <= 45, i128 n <= 105,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for the triangle A292625:
   For row r (1‑based) let A_i = 2^(i+1)+1.
   Let e_k be the elementary symmetric sums of the A_i:
        e_0 = 1,
        e_k = Σ_{1≤i1<…<ik≤r} A_{i1}…A_{ik}   (1≤k≤r).
   Let s₀ = Σ_{k=1}^r (-1)^{k-1} e_k.
   Define C = (1 - s₀)/2.
   Then the r‑th row entries y_j (1‑based) are
        y_1 = (-1)^{r-1} e_r + C,
        y_j = (-1)^{r-j} e_{r-j+1}          for j>1.
   The sequence a(n) is the flattened triangle (row‑major).  */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* index is 1‑based; offset is 1 */

    /* --- static state --------------------------------------------------- */
    static seq_t *seq = NULL;      /* flattened triangle values     */
    static size_t seq_len = 0, seq_cap = 0;

    static seq_t *e   = NULL;      /* elementary symmetric sums   */
    static size_t  e_cap = 0;

    static idx_t cur_row = 0;      /* last completed row          */

    /* --- grow seq array if needed -------------------------------------- */
    while (seq_len < (size_t)n)
    {
        /* --------- add next row ---------------------------------------- */
        ++cur_row;                     /* new row number   */
        idx_t i = cur_row;

        /* ensure e has space for index i */
        if (e_cap <= (size_t)i)
        {
            size_t new_cap = e_cap ? e_cap << 1 : 4;
            while (new_cap <= (size_t)i) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(e, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failure */
            if (!e) {
                /* initialise with zeros */
                for (size_t k = 0; k < new_cap; ++k) tmp[k] = (seq_t)0;
                tmp[0] = (seq_t)1;               /* e_0 = 1 */
            }
            e = tmp;
            e_cap = new_cap;
        }

        /* compute A_i = 2^(i+1)+1 */
        seq_t Ai = pow2((idx_t)(i + 1));
        Ai = Ai + (seq_t)1;

        /* update elementary symmetric sums: e_k += A_i * e_{k-1} */
        for (idx_t k = i; k >= 1; --k)
        {
            e[k] = e[k] + Ai * e[k-1];
        }

        /* compute s0 = Σ_{k=1}^i (-1)^{k-1} e_k */
        seq_t s0 = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
        {
            if ((k & 1) != 0)
                s0 = s0 + e[k];
            else
                s0 = s0 - e[k];
        }

        /* compute C = (1 - s0)/2 */
        seq_t C = ((seq_t)1 - s0) / (seq_t)2;

        /* append the row to seq */
        if (seq_len + (size_t)i > seq_cap)
        {
            size_t new_cap = seq_cap ? seq_cap << 1 : 8;
            while (new_cap < seq_len + (size_t)i) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            seq = tmp;
            seq_cap = new_cap;
        }

        /* first element y1 */
        seq_t y1;
        if (( (i-1) & 1 ) != 0)
            y1 = -e[i] + C;
        else
            y1 = e[i] + C;
        seq[seq_len++] = y1;

        /* remaining elements y_j (j>1) */
        for (idx_t j = 2; j <= i; ++j)
        {
            idx_t k = i - j + 1;   /* corresponds to e_k    */
            seq_t val;
            if ( ((i-j) & 1) != 0 )
                val = -e[k];
            else
                val = e[k];
            seq[seq_len++] = val;
        }
    }

    return seq[n-1];
}
