/*
 * A302304  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302304:
   a(n) = 3*a(n-1) - 2*a(n-2) +13*a(n-3) -23*a(n-4) +13*a(n-5)
          -42*a(n-6) +25*a(n-7) -21*a(n-8) -36*a(n-9) +48*a(n-10)
          -24*a(n-11) +8*a(n-12) +32*a(n-13)   for n > 16
   Initial values:
   a(1)=3, a(2)=11, a(3)=13, a(4)=33, a(5)=58, a(6)=153,
   a(7)=336, a(8)=937, a(9)=2449, a(10)=6560, a(11)=18488,
   a(12)=52053, a(13)=147896, a(14)=425764, a(15)=1229112,
   a(16)=3561761.  Implementation uses the recurrence
   with the first 16 terms pre‑computed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, return 0 */

    /* first 16 terms fixed */
    static const seq_t init[16] = {
        (seq_t)3, (seq_t)11, (seq_t)13, (seq_t)33, (seq_t)58,
        (seq_t)153, (seq_t)336, (seq_t)937, (seq_t)2449,
        (seq_t)6560, (seq_t)18488, (seq_t)52053, (seq_t)147896,
        (seq_t)425764, (seq_t)1229112, (seq_t)3561761
    };

    if (n <= 16) return init[n-1];

    /* allocate temporary array a[0..n] */
    seq_t *arr;
#ifdef __cplusplus
    arr = new seq_t[(size_t)n + 1];
#else
    arr = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) abort();
#endif

    /* initialise precomputed values */
    for (idx_t i = 1; i <= 16; ++i) {
        arr[i] = init[i-1];
    }

    /* compute using the recurrence for i = 17 .. n */
    for (idx_t i = 17; i <= n; ++i) {
        seq_t val = 3 * arr[i-1];
        val = val - 2 * arr[i-2];
        val = val + 13 * arr[i-3];
        val = val - 23 * arr[i-4];
        val = val + 13 * arr[i-5];
        val = val - 42 * arr[i-6];
        val = val + 25 * arr[i-7];
        val = val - 21 * arr[i-8];
        val = val - 36 * arr[i-9];
        val = val + 48 * arr[i-10];
        val = val - 24 * arr[i-11];
        val = val + 8  * arr[i-12];
        val = val + 32 * arr[i-13];
        arr[i] = val;
    }

    seq_t result = arr[n];

#ifndef __cplusplus
    free(arr);
#else
    delete[] arr;
#endif

    return result;
}
