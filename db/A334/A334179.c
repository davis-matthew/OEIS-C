/*
 * A334179  Number of dimer tilings of a 2*n x 6 Moebius strip.
 * https://oeis.org/A334179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 52*a(n-1) - 673*a(n-2) + 2548*a(n-3) - 3856*a(n-4) +
 *            2548*a(n-5) - 673*a(n-6) + 52*a(n-7) - a(n-8) Initial values:
 *            a(0)=1, a(1)=18, a(2)=539, a(3)=17753, a(4)=603126,
 *            a(5)=20721019, a(6)=714790675, a(7)=24693540102
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial terms of the sequence */
    static const seq_t init[8] = {
        (seq_t)1ULL,
        (seq_t)18ULL,
        (seq_t)539ULL,
        (seq_t)17753ULL,
        (seq_t)603126ULL,
        (seq_t)20721019ULL,
        (seq_t)714790675ULL,
        (seq_t)24693540102ULL
    };

    if (n < 8)
        return init[(size_t)n];

    /* keep the last 8 values in a circular buffer */
    seq_t buf[8];
    /* buf[0]=a(n-7), buf[7]=a(n-1) for the current window */
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* iterate from 8 up to n */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* compute a(i) from the 8 previous terms in buf */
        seq_t next = (seq_t)52 * buf[7]            /* + 52*a(i-1)    */
                   - (seq_t)673 * buf[6]           /* - 673*a(i-2)   */
                   + (seq_t)2548 * buf[5]          /* + 2548*a(i-3)  */
                   - (seq_t)3856 * buf[4]          /* - 3856*a(i-4)  */
                   + (seq_t)2548 * buf[3]          /* + 2548*a(i-5)  */
                   - (seq_t)673 * buf[2]           /* - 673*a(i-6)   */
                   + (seq_t)52 * buf[1]            /* + 52*a(i-7)    */
                   - buf[0];                      /* -   a(i-8)     */

        /* shift the buffer for the next iteration */
        for (size_t j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = next;
    }

    /* after the loop buf[7] holds a(n) */
    return buf[7];
}
