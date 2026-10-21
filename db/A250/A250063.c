/*
 * A250063  Number of length 5+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A250063
   Formula:  a(n) = 12*a(n-1) - 66*a(n-2) + 220*a(n-3) - 495*a(n-4)
                 + 792*a(n-5) - 924*a(n-6) + 792*a(n-7) - 495*a(n-8)
                 + 220*a(n-9) - 66*a(n-10) + 12*a(n-11) - a(n-12)   (n>12)
   Initial values (n=1..12):
        21, 7054, 348066, 6415638, 65419155, 449532868, 2330996068,
        9790907652, 34925259201, 109359675810, 307898807590, 793597679082
*/
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is valid, a(0) is unused */
    if (n == 0)
        return 0;

    /* Initial 12 terms, stored in 0‑based array */
    static const seq_t init[12] = {
        21ull,
        7054ull,
        348066ull,
        6415638ull,
        65419155ull,
        449532868ull,
        2330996068ull,
        9790907652ull,
        34925259201ull,
        109359675810ull,
        307898807590ull,
        793597679082ull
    };

    if (n <= 12)
        return init[n-1];

    /* Sliding window of the last 12 values.
       buf[0] is a(k-12), buf[11] is a(k-1) for current k. */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute subsequent terms up to n */
    for (idx_t k = 13; k <= n; ++k) {
        /* Positive contributions */
        seq_t pos = 12 * buf[11];                    /* a(k-1)  */
        pos += 220 * buf[9];                        /* a(k-3)  */
        pos += 792 * buf[7];                        /* a(k-5)  */
        pos += 792 * buf[5];                        /* a(k-7)  */
        pos += 220 * buf[3];                        /* a(k-9)  */
        pos += 12 * buf[1];                         /* a(k-11) */

        /* Negative contributions */
        seq_t neg = 66 * buf[10];                   /* a(k-2)  */
        neg += 495 * buf[8];                        /* a(k-4)  */
        neg += 924 * buf[6];                        /* a(k-6)  */
        neg += 495 * buf[4];                        /* a(k-8)  */
        neg += 66 * buf[2];                         /* a(k-10) */
        neg += 1 * buf[0];                          /* a(k-12) */

        /* Next value */
        seq_t next = pos - neg;

        /* Shift window: drop oldest, push new value */
        for (size_t j = 0; j < 11; ++j)
            buf[j] = buf[j+1];
        buf[11] = next;
    }

    /* After the loop, buf[11] holds a(n) */
    return buf[11];
}
