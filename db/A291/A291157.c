/*
 * A291157  Sequence of black numbers on the single zero (European) roulette
 *          wheel read clockwise.
 * https://oeis.org/A291157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th black number on a single‑zero (European)
 *            roulette wheel read clockwise
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is periodic with period 18. */

seq_t a(idx_t n)
{
    static const seq_t tbl[18] = {
        15, 4, 2, 17, 6, 13, 11, 8, 10, 24,
        33, 20, 31, 22, 29, 28, 35, 26
    };
    idx_t i = (n - 1) % 18;   /* 0‑based index */
    return tbl[i];
}
