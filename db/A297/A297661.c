/*
 * A297661  a(n) = n + 2*cos((n*Pi)/3) + Lucas(n).
 * https://oeis.org/A297661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - 2*a(n-5) + a(n-6),
 *            a(1)=3, a(2)=4, a(3)=5, a(4)=10, a(5)=17, a(6)=26
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the defined offset (n >= 1) */
    if (n == 0) return (seq_t)0;            /* undefined, return 0 as safe default */

    /* Initialisation for the first six terms */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)17;
    if (n == 6) return (seq_t)26;

    /* Sliding window of the last six values:
       arr[0] = a(n-1), arr[1] = a(n-2), ..., arr[5] = a(n-6)   */
    seq_t arr[6] = { (seq_t)26, (seq_t)17, (seq_t)10,
                     (seq_t)5,  (seq_t)4,  (seq_t)3 };

    seq_t val = 0;
    for (idx_t i = 7; i <= n; ++i)
    {
        /* Compute a(i) using the recurrence */
        val = (seq_t)4 * arr[0]
              - (seq_t)6 * arr[1]
              + (seq_t)4 * arr[2]
              - (seq_t)2 * arr[4]
              +       arr[5];

        /* Shift the window for the next iteration:
           arr[5] <-- arr[4], arr[4] <-- arr[3], ... , arr[1] <-- arr[0], arr[0] <-- val */
        for (int k = 5; k > 0; --k)
            arr[k] = arr[k-1];
        arr[0] = val;
    }

    return val;         /* val now holds a(n) */
}
