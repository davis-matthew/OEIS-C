/*
 * A345486  Numbers that are the sum of seven squares in nine or more ways.
 * https://oeis.org/A345486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1..13) are listed; for n > 13 the conjectured recurrence a(n)
 *            = 2*a(n-1) - a(n-2) holds, which reduces to a(n) = n + 81
 * Range:     u8 n <= 174, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        69, 70, 78, 79, 81, 82, 85, 87, 88, 90, 91, 93, 94
    };

    if (n == 0) {
        /* Undefined for n=0; return 0 as a placeholder. */
        return (seq_t)0;
    }
    if (n <= 13) {
        return init[n - 1];
    }
    /* For n > 13, use the linear pattern a(n) = n + 81. */
    return (seq_t)n + (seq_t)81;
}
