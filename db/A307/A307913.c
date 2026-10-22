/*
 * A307913  Numbers without the decimal digits 3, 6 and 9.
 * https://oeis.org/A307913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = map_digits(n-1) from base 7 to decimal digits * 0→0, 1→1,
 *            2→2, 3→4, 4→5, 5→7, 6→8 * (e.g., a(4)=4, a(8)=10, a(22)=40 etc.)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset n=1 → a(1)=0 */
    if (n == 0)
        return (seq_t)0;

    idx_t m = n - 1;                  /* work with n-1 in base 7 */
    /* Find highest power of 7 <= m */
    idx_t power = 1;
    while (power <= m / 7)
        power *= 7;

    static const unsigned char map[7] = {0, 1, 2, 4, 5, 7, 8};

    seq_t result = (seq_t)0;
    while (power > 0) {
        unsigned digit = (unsigned)(m / power);   /* 0 <= digit <= 6 */
        m %= power;
        result = result * 10 + (seq_t)map[digit];
        power /= 7;
    }
    return result;
}
