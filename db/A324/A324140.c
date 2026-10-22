/*
 * A324140  Define b(n) and c(n) by b(0)=0, b(1)=c(0)=c(1)=1;
 *          b(n)=c(n-1)*c(n-2), c(n) = (b(n-1)+c(n-1))*(b(n-2)+c(n-2));
 *          sequence gives b(n).
 * https://oeis.org/A324140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b(0)=0, b(1)=1, c(0)=1, c(1)=1;
 * b(n)=c(n-1)*c(n-2);
 * c(n)=(b(n-1)+c(n-1))*(b(n-2)+c(n-2));
 * sequence a(n) returns b(n).  Works for any idx_t n ≥ 0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *B = NULL, *C = NULL;
    static size_t len = 0;            /* number of elements currently stored */

    /* initialisation for the first call */
    if (len == 0)
    {
        len = 2;                     /* we need indices 0 and 1 at start */
        B = (seq_t *)realloc(B, len * sizeof(seq_t));
        C = (seq_t *)realloc(C, len * sizeof(seq_t));
        B[0] = (seq_t)0;  C[0] = (seq_t)1;
        B[1] = (seq_t)1;  C[1] = (seq_t)1;
    }

    /* grow the arrays if the requested index is beyond current extent */
    if ((size_t)n >= len)
    {
        size_t old_len = len;
        size_t required = (size_t)n + 1;           /* indices are 0‑based */
        B = (seq_t *)realloc(B, required * sizeof(seq_t));
        C = (seq_t *)realloc(C, required * sizeof(seq_t));

        for (size_t i = old_len; i < required; ++i)
        {
            seq_t b_nm1 = B[i - 1];
            seq_t c_nm1 = C[i - 1];
            seq_t b_nm2 = B[i - 2];
            seq_t c_nm2 = C[i - 2];
            B[i] = c_nm1 * c_nm2;
            C[i] = (b_nm1 + c_nm1) * (b_nm2 + c_nm2);
        }
        len = required;
    }

    return B[(size_t)n];
}
