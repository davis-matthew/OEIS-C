/*
 * A280640  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 0.
 * https://oeis.org/A280640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 301, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280640: numbers k such that k³ has an odd number of digits
   and the middle digit of its decimal representation is 0.
   Computed by brute‑force search. */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for 0 */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        /* compute k³ (fits into seq_t for the required k that
           produce a term within the range of seq_t) */
        seq_t cube = (seq_t)k * k * k;

        int len = digit_len(cube);              /* number of decimal digits */
        if (len & 1)                            /* odd number of digits?    */
        {
            int exp   = len >> 1;                /* position of middle digit */
            seq_t pow10e = pow10((idx_t)exp);   /* 10^exp */
            seq_t middle  = (cube / pow10e) % 10;/* middle digit            */
            if (middle == 0)
            {
                ++count;
                if (count == n) return (seq_t)k;  /* found the n-th term   */
            }
        }
    }
}
