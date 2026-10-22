/*
 * A304440  Add to n the sum of its even digits minus the sum of its odd
 *          digits.
 * https://oeis.org/A304440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + Σ_{d∈digits(n)} (d if d even else –d)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t even_sum = 0;
    seq_t odd_sum  = 0;
    idx_t m = n;

    /* Handle the number 0 separately: no digits loop is needed. */
    while (m > 0) {
        unsigned d = m % 10;      /* one decimal digit, 0–9 */
        if ((d & 1) == 0)         /* even digit */
            even_sum += d;
        else
            odd_sum += d;
        m /= 10;
    }

    return (seq_t)n + even_sum - odd_sum;
}
