/*
 * A353146  Decimal repunits written in base 7.
 * https://oeis.org/A353146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007093(A002275(n))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   decimal repunits written in base 7 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Build the decimal repunit of length n : R = 11…1 (n digits) */
    seq_t rep = (seq_t)0;
    for (idx_t i = 0; i < n; ++i)
        rep = rep * (seq_t)10 + (seq_t)1;

    /* Convert R to base‑7 and read the digits as a decimal number */
    seq_t res       = (seq_t)0;
    seq_t ten_power = (seq_t)1;          /* 10^k for the current digit place */

    while (rep != (seq_t)0) {
        seq_t digit = rep % (seq_t)7;    /* next base‑7 digit */
        rep /= (seq_t)7;
        res += digit * ten_power;       /* add this digit to the decimal result */
        ten_power *= (seq_t)10;         /* move to the next decimal position */
    }
    return res;
}
