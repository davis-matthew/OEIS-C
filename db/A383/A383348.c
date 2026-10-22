/*
 * A383348  Triangle related to the partitions of n in three colors, read by
 *          rows.
 * https://oeis.org/A383348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) follows the triangle defined by T(i,j) = 27*T(i-1,j-1) +
 *            9*T(i-2,j-1) + T(i-3,j-1) with seeds T(0,0)=9, T(1,0)=6,
 *            T(2,0)=1; all other entries with j=0 are 0. a(n) is the n‑th
 *            element read row‑by‑row (offset 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 34, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

#if defined(__cplusplus)
inline seq_t* alloc_row(idx_t len) { return new seq_t[(size_t)len]; }
inline void  free_row(seq_t* p)      { delete[] p; }
#else
static inline seq_t* alloc_row(idx_t len) { return (seq_t*)malloc((size_t)len * sizeof(seq_t)); }
static inline void  free_row(seq_t* p)      { free(p); }
#endif

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for offset <1 */

    /* Determine the row r and column c such that a(n) = T(r,c) */
    idx_t r = 0, cumulative = 1;
    while (n > cumulative) {
        ++r;
        cumulative += r + 1;     /* cumulative terms up to row r */
    }
    idx_t totalPrev = cumulative - (r + 1);  /* terms before row r */
    idx_t c = n - totalPrev - 1;             /* column in row r */

    /* Dynamic programming: keep the last three rows */
    seq_t *prev3 = NULL, *prev2 = NULL, *prev1 = NULL;
    idx_t lenPrev1 = 0, lenPrev2 = 0, lenPrev3 = 0;

    for (idx_t i = 0; i <= r; ++i) {
        seq_t *cur = alloc_row(i + 1);

        for (idx_t j = 0; j <= i; ++j) {
            if ((i == 0 && j == 0) || (i == 1 && j == 0) || (i == 2 && j == 0)) {
                if (i == 0) cur[j] = (seq_t)9;
                else if (i == 1) cur[j] = (seq_t)6;
                else /* i==2 */ cur[j] = (seq_t)1;
            } else if (j == 0) {
                cur[j] = (seq_t)0;                     /* base for i>=3 */
            } else {
                seq_t a1 = (lenPrev1 && j-1 < lenPrev1) ? prev1[j-1] : (seq_t)0;
                seq_t a2 = (lenPrev2 && j-1 < lenPrev2) ? prev2[j-1] : (seq_t)0;
                seq_t a3 = (lenPrev3 && j-1 < lenPrev3) ? prev3[j-1] : (seq_t)0;
                cur[j] = 27 * a1 + 9 * a2 + a3;
            }
        }

        if (i == r) {
            seq_t result = cur[c];
            free_row(prev3);
            free_row(prev2);
            free_row(prev1);
            free_row(cur);
            return result;
        }

        free_row(prev3);
        prev3 = prev2;  lenPrev3 = lenPrev2;
        prev2 = prev1;  lenPrev2 = lenPrev1;
        prev1 = cur;    lenPrev1 = i + 1;
    }

    /* Should never reach here */
    return (seq_t)0;
}
