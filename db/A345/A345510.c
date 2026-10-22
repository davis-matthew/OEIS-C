/*
 * A345510  Numbers that are the sum of ten squares in three or more ways.
 * https://oeis.org/A345510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 40 for n > 7; a(1) = 34, a(2) = 37, a(3) = 40, a(4) =
 *            42, a(5) = 43, a(6) = 45, a(7) = 46
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the seven irregular terms explicitly */
    static const seq_t first[7] = {
        /* a(1) */ 34,
        /* a(2) */ 37,
        /* a(3) */ 40,
        /* a(4) */ 42,
        /* a(5) */ 43,
        /* a(6) */ 45,
        /* a(7) */ 46
    };

    if (n == 0) {
        /* Undefined for n = 0; return 0 as a safe placeholder */
        return (seq_t)0;
    }

    if (n <= 7) {
        return first[n - 1];
    }

    /* For n > 7, use the linear formula */
    return (seq_t)n + (seq_t)40;
}
