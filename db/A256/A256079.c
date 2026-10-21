/*
 * A256079  Increase each (decimal) digit of n by 1, with carry (i.e., '9'
 *          becomes '0' and a (further) increment of 1 of the digit to the
 *          left).
 * https://oeis.org/A256079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + repunit(number_of_digits(n))
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n + (10^d - 1) / 9, where d = #decimal digits of n */

/* Count decimal digits of an unsigned 64‑bit number. */
static idx_t digit_count_idx(idx_t n)
{
    if (n == 0) return 1;
    idx_t cnt = 0;
    while (n > 0) {
        n /= 10;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Find number of decimal digits of n. */
    idx_t d = digit_count_idx(n);

    /* Build the repunit 111…1 of length d. */
    seq_t rep = 0;
    for (idx_t i = 0; i < d; ++i)
        rep = rep * 10 + 1;

    /* Return n + repunit. */
    return (seq_t)n + rep;
}
