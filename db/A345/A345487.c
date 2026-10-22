/*
 * A345487  Numbers that are the sum of seven squares in ten or more ways.
 * https://oeis.org/A345487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 84 for n ≥ 9; initial values: a(1)=70, a(2)=79,
 *            a(3)=82, a(4)=85, a(5)=87, a(6)=88, a(7)=90, a(8)=91, a(9)=93
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* protect against n < 1; behaviour for n == 0 is irrelevant */
    if (n == 0) return 0;

    /* first eight terms */
    if (n <= 8) {
        static const seq_t init[8] = {70,79,82,85,87,88,90,91};
        return init[n-1];
    }

    /* n == 9 */
    if (n == 9) return 93;

    /* linear recurrence part: a(n) = n + 84 for n >= 10 */
    return (seq_t)n + (seq_t)84;
}
