/*
 * A291159  Sequence of red numbers on the double zero (American) roulette
 *          wheel read clockwise.
 * https://oeis.org/A291159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is periodic modulo 18 with cycle *
 *            9,30,7,32,5,34,3,36,1,27,25,12,19,18,21,16,23,14 * (i.e., a(n) =
 *            a(n + 18) for all n ≥ 1)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period of 18 red numbers on the American double‑zero wheel. */
    const seq_t red[18] = {
        9, 30, 7, 32, 5, 34, 3, 36,
        1, 27, 25, 12, 19, 18, 21, 16, 23, 14
    };

    /* n is 1‑based; compute zero‑based index modulo 18. */
    idx_t idx = (n - 1) % 18;
    return red[idx];
}
