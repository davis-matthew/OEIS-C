/*
 * A246435  Length of representation of n in fractional base 3/2.
 * https://oeis.org/A246435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = if n < 3 then 1, otherwise a(2*floor(n/3)) + 1. Initial
 *            values: a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the representation length is always 1. */
    if (n < 3)
        return 1;

    /* Count how many times the recurrence can be applied. */
    idx_t steps = 0;
    while (n >= 3) {
        n = (n / 3) * 2; /* floor(n/3) then multiply by 2 */
        steps++;
    }

    /* The base case contributes 1. */
    return (seq_t)(steps + 1);
}
