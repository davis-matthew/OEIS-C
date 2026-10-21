/*
 * A276138  Numbers without the decimal digits 1, 3, 5 and 7.
 * https://oeis.org/A276138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0; a(6n-6+k)=10*a(n)+a(k) for k=1..6 (OEIS A276138)
 *            Equivalent: interpret (n-1) in base‑6, map digits 0..5 to
 *            0,2,4,6,8,9, and read the result as a decimal integer
 * Range:     u8 n <= 54, u16 n <= 4536, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char digit_map[6] = {0, 2, 4, 6, 8, 9};

seq_t a(idx_t n)
{
    /* Handle the first term directly (n == 1) */
    if (n == 1)
        return 0;

    idx_t   x    = n - 1;        /* base‑6 representation of n‑1 */
    seq_t   res  = 0;            /* the resulting decimal number   */
    seq_t   power = 1;           /* 10^position of the digit       */

    while (x > 0)
    {
        unsigned int d = x % 6;          /* least significant base‑6 digit */
        res += (seq_t)digit_map[d] * power;
        power *= 10;
        x /= 6;
    }

    return res;
}
