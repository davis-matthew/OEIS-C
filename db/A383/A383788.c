/*
 * A383788  Smallest number obtainable by either keeping each decimal digit d
 *          in n or replacing it with 9-d.
 * https://oeis.org/A383788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number obtained by replacing each decimal digit d of n
 *            with min(d, 9-d) and recombining the digits. Equivalently, a(n)
 *            = Σ_{i=0}^{k-1} min(d_i, 9-d_i) * 10^{k-1-i}, where d_i are the
 *            decimal digits of n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Handle n == 0 if called (outside offset) */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Extract decimal digits from least significant to most
       into a fixed-size buffer (max 20 digits for 64-bit n). */
    unsigned int buf[20];
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        unsigned int dig = (unsigned int)(tmp % (idx_t)10);
        buf[len++] = dig;
        tmp /= (idx_t)10;
    }

    /* Construct result using most significant digit first */
    seq_t result = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned int d = buf[i];
        unsigned int alt = 9 - d;
        unsigned int v = d < alt ? d : alt;
        result = result * (seq_t)10 + (seq_t)v;
    }
    return result;
}
