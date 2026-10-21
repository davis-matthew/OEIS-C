/*
 * A254531  a(n) is the position of the piano key whose frequency is closest
 *          to n Hz, start with A0 = the 1st key.
 * https://oeis.org/A254531
 *
 * Offset:    27    (a(27) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = round(12*log_2(n/440)) + 49, 27 <= n <= 4308
 * Range:     u8 n <= 4308, u16 n <= 4308, u32 n <= 4308, u64 n <= 4308,
 *            u128 n <= 4308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 27) return 0;          /* offset not defined below 27 */
    double freq   = (double)n;
    double key    = 12.0 * log2(freq / 440.0) + 49.0; /* compute key */
    return (seq_t) llround(key);   /* round to nearest integer */
}
