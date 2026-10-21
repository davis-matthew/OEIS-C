/*
 * A296716  Numbers congruent to {7, 11, 13, 29} mod 30.
 * https://oeis.org/A296716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + 30, for n > 4; initial values a(1)=7, a(2)=11,
 *            a(3)=13, a(4)=29
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[4] = { (seq_t)7, (seq_t)11, (seq_t)13, (seq_t)29 };

    if (n <= 4)            /* n is unsigned so n == 0 is not expected */
        return base[n - 1];

    idx_t q = (n - 1) / 4; /* number of complete 4‑block cycles */
    idx_t r = (n - 1) % 4; /* position inside the current cycle */

    return ((seq_t)30 * (seq_t)q) + base[r];
}
