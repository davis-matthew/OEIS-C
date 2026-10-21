/*
 * A241621  Number of length 6+2 0..n arrays with no consecutive three
 *          elements summing to more than n.
 * https://oeis.org/A241621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
          + 126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
   for n > 9.  Initial values:
   a(1)=28  a(2)=287  a(3)=1730  a(4)=7483  a(5)=25774
   a(6)=75180  a(7)=193194  a(8)=449295  a(9)=963886   */

#include <stdlib.h>   /* needed only for C; harmless for C++ */

seq_t a(idx_t n)
{
    /* Return value for undefined index 0 */
    if (n == 0)
        return 0;

    /* Initial values for n = 1 .. 9 */
    static const seq_t init[10] = {
        0,          /* unused, index 0 */
        28, 287, 1730, 7483, 25774,
        75180, 193194, 449295, 963886
    };

    if (n <= 9)
        return init[n];

    /* 9‑element ring buffer holding the last 9 computed terms
       values[0] = a(k‑9), values[8] = a(k‑1) when computing a(k)   */
    seq_t values[9];
    for (idx_t i = 0; i < 9; ++i)
        values[i] = init[i + 1];          /* set to a(1)..a(9) */

    seq_t ak = 0;                         /* current a(k) */

    for (idx_t k = 10; k <= n; ++k) {
        /* compute a(k) from the 9 previous values */
        ak = (seq_t)9   * values[8]
            - (seq_t)36  * values[7]
            + (seq_t)84  * values[6]
            - (seq_t)126 * values[5]
            + (seq_t)126 * values[4]
            - (seq_t)84  * values[3]
            + (seq_t)36  * values[2]
            - (seq_t)9   * values[1]
            + values[0];

        /* shift the ring buffer left by one position */
        for (idx_t i = 0; i < 8; ++i)
            values[i] = values[i + 1];
        values[8] = ak;                  /* now holds a(k) */
    }

    return ak;
}
