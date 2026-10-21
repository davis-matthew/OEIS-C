/*
 * A253923  Octagonal numbers (A000567) which are also centered pentagonal
 *          numbers (A005891).
 * https://oeis.org/A253923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+232322*a(n-2)-232322*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=1, a(2)=7701, a(3)=219781, a(4)=1789106881,
 *            a(5)=51059956641
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   mul232322
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc. */
#include "../utils/mul232322.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term. */
    if (n < 1) return 0;

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7701;
    if (n == 3) return (seq_t)219781;
    if (n == 4) return (seq_t)1789106881ULL;
    if (n == 5) return (seq_t)51059956641ULL;

    /* Store the five most recent terms.
       arr[0] = a(k-5), … , arr[4] = a(k-1). */
    seq_t arr[5] = {
        (seq_t)1,                /* a(1) */
        (seq_t)7701,             /* a(2) */
        (seq_t)219781,           /* a(3) */
        (seq_t)1789106881ULL,    /* a(4) */
        (seq_t)51059956641ULL    /* a(5) */
    };

    for (idx_t i = 6; i <= n; ++i) {
        /* next = a(i-1) + 232322*a(i-2) - 232322*a(i-3)
                 - a(i-4) + a(i-5) */
        seq_t next = arr[4]
                     + mul232322(arr[3])
                     - mul232322(arr[2])
                     - arr[1]
                     + arr[0];

        /* Shift window: drop oldest term, add new term. */
        arr[0] = arr[1];
        arr[1] = arr[2];
        arr[2] = arr[3];
        arr[3] = arr[4];
        arr[4] = next;
    }

    return arr[4];
}
