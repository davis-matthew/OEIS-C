/*
 * A284622  {0011->0}-transform of the Thue-Morse word A010060.
 * https://oeis.org/A284622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A284622: 0011→0 transform of the Thue–Morse word.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not part of the sequence */

    idx_t out_cnt = 0;                /* number of output bits produced */
    idx_t i = 0;                      /* index in the Thue–Morse word (0‑based) */

    unsigned char buf[4];             /* circular buffer holding up to 4 input bits */
    int buf_len = 0;                  /* current number of bits stored */

    while (out_cnt < n)
    {
        /* next Thue–Morse bit: parity of popcount(i) */
        unsigned char bit = (unsigned char)__builtin_parityll((uint64_t)i);
        buf[buf_len++] = bit;
        ++i;

        /* process as long as we have at least 4 bits to decide */
        while (buf_len >= 4)
        {
            if (buf[0] == 0 && buf[1] == 0 && buf[2] == 1 && buf[3] == 1)
            {
                /* pattern 0011 found → replace by a single 0 */
                ++out_cnt;
                if (out_cnt == n) return (seq_t)0;

                /* shift the remaining bits left by four positions */
                for (int k = 0; k < buf_len - 4; ++k) buf[k] = buf[k + 4];
                buf_len -= 4;
            }
            else
            {
                /* output the first bit */
                ++out_cnt;
                if (out_cnt == n) return (seq_t)buf[0];

                /* shift the remaining bits left by one position */
                for (int k = 0; k < buf_len - 1; ++k) buf[k] = buf[k + 1];
                --buf_len;
            }
        }
    }

    /* should never reach here */
    return (seq_t)0;
}
