/*
 * A274642  Numbers of the form 4^k*(8*j+7) that have exactly three partitions
 *          into four positive squares.
 * https://oeis.org/A274642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-3) for n > 9, with a(1) = 28, a(2) = 55, a(3) = 60,
 *            a(4) = 79, a(5) = 92, a(6) = 95, a(7) = 112, a(8) = 240, a(9) =
 *            368
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 44, u64 n <= 92, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values up to n = 9 */
    static const seq_t init[9] = {
        28, 55, 60, 79, 92, 95, 112, 240, 368
    };

    if (n == 0) return 0;          /* undefined for n=0 – return 0 */
    if (n <= 9) return init[n-1];

    /* Compute the number of times the recurrence has to be applied.
       After applying 4*a(n-3) e times we reach an index b with 1 ≤ b ≤ 9. */
    idx_t e = (n - 10) / 3 + 1;     /* e ≥ 1 for n > 9 */
    idx_t b = n - 3 * e;            /* 1 ≤ b ≤ 9 */

    /* Apply the recurrence by multiplying by 4 e times. */
    seq_t res = init[b-1];
    for (idx_t k = 0; k < e; ++k) {
        res *= 4;
    }
    return res;
}
