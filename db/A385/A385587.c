/*
 * A385587  Galileo sequence with ratio k = 4: a(1) = 1, a(2) = k, a(2*n-1) =
 *          floor(((k + 1)*a(n) -1)/2), and a(2*n) = floor((k + 1)*a(n)/2) + 1
 *          for n > 2.
 * https://oeis.org/A385587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=1, a(2)=4; for n>2 * a(2n-1)=floor((5*a(n)-1)/2) * a(2n)
 *            =floor(5*a(n)/2)+1
 * Range:     u8 n <= 32, u16 n <= 2048, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    // Allocate array [1..n]
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;  // allocation failure, return 0

    arr[1] = (seq_t)1;
    arr[2] = (seq_t)4;

    for (idx_t p = 2; /*no init*/; ++p)
    {
        idx_t left_idx = 2 * p - 1;
        if (left_idx > n) break;          // no more children needed

        seq_t parent = arr[p];

        seq_t left = ((seq_t)5 * parent - (seq_t)1) / (seq_t)2;
        arr[left_idx] = left;

        idx_t right_idx = 2 * p;
        if (right_idx > n) continue;

        seq_t right = ((seq_t)5 * parent) / (seq_t)2 + (seq_t)1;
        arr[right_idx] = right;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
