/*
 * A248069  Number of length 1+5 0..n arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 5*a(n-3) + 5*a(n-4) - 9*a(n-5) +
 *            5*a(n-6) - a(n-7) Initial values: a(1)=32, a(2)=333, a(3)=1804,
 *            a(4)=6545, a(5)=18636, a(6)=44677, a(7)=94568
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 60, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the offset (a(1) is the first term) */
    if (n == 0)
        return 0;               /* defined arbitrarily – below offset */

    /* Pre‑computed first seven terms */
    static const seq_t init[8] = {
        0,               /* unused for index 0 */
        32, 333, 1804, 6545, 18636, 44677, 94568
    };

    if (n <= 7)
        return init[n];

    /* Sliding window of the last seven values
       arr[0] = a(k-7), arr[1] = a(k-6), … , arr[6] = a(k-1)   */
    seq_t arr[7] = { init[1], init[2], init[3], init[4],
                     init[5], init[6], init[7] };

    for (idx_t k = 8; k <= n; ++k) {
        seq_t next =
            5 * arr[6]             /* 5*a(k-1) */
          - 9 * arr[5]             /* -9*a(k-2) */
          + 5 * arr[4]             /* +5*a(k-3) */
          + 5 * arr[3]             /* +5*a(k-4) */
          - 9 * arr[2]             /* -9*a(k-5) */
          + 5 * arr[1]             /* +5*a(k-6) */
          -  arr[0];               /* -a(k-7) */

        /* Shift the window to the left */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = arr[5];
        arr[5] = arr[6];
        arr[6] = next;
    }
    return arr[6];   /* This holds a(n) after the loop */
}
