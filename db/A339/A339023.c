/*
 * A339023  Replace each digit d in the decimal representation of n with the
 *          digital root of n*d.
 * https://oeis.org/A339023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   replace each decimal digit d of n with the digital root of n*d
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n mod 9 needed only for computing digital roots */
    idx_t n_mod9 = n % 9;

    seq_t result = 0;
    seq_t pow10 = 1;
    idx_t tmp = n;

    while (tmp > 0) {
        unsigned d = tmp % 10;        /* current decimal digit */
        seq_t r;                      /* replacement digit */

        if (d == 0)
            r = 0;
        else {
            unsigned prod_mod9 = (unsigned)((n_mod9 * d) % 9);
            r = (prod_mod9 == 0) ? 9 : prod_mod9;
        }

        result += r * pow10;
        pow10 *= 10;
        tmp /= 10;
    }

    return result;
}
