/*
 * A265068  Coordination sequence for (2,5,infinity) tiling of hyperbolic
 *          plane.
 * https://oeis.org/A265068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 101,
 *            u128 n <= 205, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5)  for n >= 7
   Initial values: a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=13, a(5)=20, a(6)=30 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t base[7] = { (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)8,
                                   (seq_t)13, (seq_t)20, (seq_t)30 };
    if (n < 7) return base[n];

    /* allocate an array of length n+1; n is at most order 10⁴ in tests */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    arr[0] = base[0];
    arr[1] = base[1];
    arr[2] = base[2];
    arr[3] = base[3];
    arr[4] = base[4];
    arr[5] = base[5];
    arr[6] = base[6];

    for (idx_t i = 7; i <= n; ++i) {
        arr[i] = arr[i - 2] + arr[i - 3] + arr[i - 4] + arr[i - 5];
    }

    seq_t res = arr[n];
    free(arr);
    return res;
}
