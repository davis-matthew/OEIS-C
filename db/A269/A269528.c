/*
 * A269528  Parity of number of runs of 1's in negabinary representation of n.
 * https://oeis.org/A269528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1999, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of the number of runs of 1's in the negabinary representation of n */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    __int128 x = n;           /* signed intermediate for the negabinary conversion */
    unsigned parity = 0;       /* 0 or 1 – result to be returned   */
    bool prev1 = false;        /* whether previous digit was a 1   */

    while (x != 0) {
        unsigned bit = (unsigned)(x & 1);   /* current least‑significant digit */

        if (bit) {                          /* digit is 1 */
            if (!prev1) {                   /* start of a new run of 1's */
                parity ^= 1;                /* toggle parity                 */
                prev1 = true;
            }
        } else {                            /* digit is 0 */
            prev1 = false;                  /* close any preceding run       */
        }

        x = (x - bit) / (-2);               /* shift to next digit in base -2 */
    }

    return (seq_t)parity;
}
