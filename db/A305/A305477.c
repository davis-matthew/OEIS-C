/*
 * A305477  Number of nX3 0..1 arrays with every element unequal to 0, 1, 3, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 2*a(n-6) for n>9, with a(1)=3,
 *            a(2)=7, a(3)=10, a(4)=21, a(5)=40, a(6)=70, a(7)=125, a(8)=231,
 *            a(9)=419
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety: offset is 1 */

    const idx_t init_limit = 9;
    static const seq_t init_vals[10] = {          /* 0 unused, indices 1‑9 filled */
        (seq_t)0,
        (seq_t)3,  (seq_t)7,  (seq_t)10, (seq_t)21,
        (seq_t)40, (seq_t)70, (seq_t)125,(seq_t)231,
        (seq_t)419
    };

    /* allocate an array to hold values up to n */
    size_t sz = (size_t)(n + 1);
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr)
        abort();

    /* base cases */
    arr[0] = init_vals[0];
    if (n <= init_limit) {
        seq_t res = init_vals[n];
        free(arr);
        return res;
    }
    for (idx_t i = 1; i <= init_limit; ++i)
        arr[i] = init_vals[i];

    /* recurrence for remaining terms */
    for (idx_t i = init_limit + 1; i <= n; ++i) {
        arr[i] = arr[i - 1] + (seq_t)3 * arr[i - 3] - (seq_t)2 * arr[i - 6];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
