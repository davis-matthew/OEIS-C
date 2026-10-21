/*
 * A269248  Number of times the digit 8 appears in the decimal expansion of
 *          n^3.
 * https://oeis.org/A269248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of times the digit 8 appears in the decimal
 *            expansion of n^3
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^3 using seq_t arithmetic */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* Count digit '8' in the decimal representation of cube */
    uint64_t cnt = 0;
    seq_t t = cube;
    while (t > 0) {
        if ((t % 10) == 8) {
            ++cnt;
        }
        t /= 10;
    }

    return (seq_t)cnt;
}
