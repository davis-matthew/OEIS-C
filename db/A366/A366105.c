/*
 * A366105  a(n) is the number of parts in the n-th partition of n when the
 *          partitions are listed in graded reverse lexicographic order (cf.
 *          A080577, as in Mathematica).
 * https://oeis.org/A366105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Computes a(n) as the number of parts in the n‑th partition of n
   when partitions of n are listed in reverse lexicographic order
   (i.e., lexicographically descending by parts).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for 0; return 0 just in case */
        return 0;

    /* Allocate array for parts; maximum possible parts is n */
    seq_t *parts = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!parts)
        return 0;                /* allocation failure */

    /* Start with the partition [n] */
    int k = 1;                   /* number of parts currently stored */
    parts[0] = (seq_t)n;
    idx_t cnt = 1;               /* number of partitions generated so far */

    if (cnt == n) {
        free(parts);
        return 1;
    }

    while (cnt < n) {
        /* Find the rightmost part that is greater than 1 */
        int j = k - 1;
        while (j >= 0 && parts[j] == 1)
            j--;

        /* In practice j will always be valid because p(n) > n */
        if (j < 0)
            break;   /* all parts are 1: last partition reached */

        /* Decrease part j by 1 */
        parts[j] -= 1;

        /* Compute the remainder to redistribute: the 1 we removed
           plus all parts that were after position j */
        seq_t rem = 1;
        for (int t = j + 1; t < k; t++)
            rem += parts[t];

        /* Retain parts up to position j */
        k = j + 1;

        /* Redistribute remainder into parts not exceeding parts[j] */
        while (rem > parts[j]) {
            parts[k++] = parts[j];
            rem -= parts[j];
        }
        parts[k++] = rem;  /* final part */

        cnt++;              /* another partition produced */
    }

    idx_t result = (idx_t)k;   /* number of parts in the n-th partition */
    free(parts);
    return (seq_t)result;
}
