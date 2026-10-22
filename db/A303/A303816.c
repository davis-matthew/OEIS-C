/*
 * A303816  Decimal expansion of 2700/17.
 * https://oeis.org/A303816
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit of the decimal expansion of 2700/17 (the
 *            first digit of the integer part is a(3))
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t DECIMAL_PERIOD[16] = {
    (seq_t)8, (seq_t)2, (seq_t)3, (seq_t)5,
    (seq_t)2, (seq_t)9, (seq_t)4, (seq_t)1,
    (seq_t)1, (seq_t)7, (seq_t)6, (seq_t)4,
    (seq_t)7, (seq_t)0, (seq_t)5, (seq_t)8
};

seq_t a(idx_t n)
{
    /* Integer part of 2700/17 is 158 */
    if (n == 3u) return (seq_t)1;
    if (n == 4u) return (seq_t)5;
    if (n == 5u) return (seq_t)8;

    /* Decimal part repeats every 16 digits */
    idx_t pos = n - 6u;               /* zero‑based index in the period  */
    return DECIMAL_PERIOD[pos & 15u]; /* pos % 16, using bitmask for speed */
}
