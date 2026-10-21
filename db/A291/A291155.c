/*
 * A291155  Sequence of red numbers on the single zero (European) roulette
 *          wheel read clockwise.
 * https://oeis.org/A291155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = wheel_map[(n-1) % 18]
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of red numbers on the single zero (European) roulette wheel read clockwise. */
/* The wheel is periodic with period 18: 32,19,21,25,34,27,36,30,23,5,16,1,14,9,18,7,12,3. */
seq_t a(idx_t n)
{
    /* static table of the 18 red numbers in clockwise order starting from 32 */
    static const uint8_t wheel_map[18] = {
        32, 19, 21, 25, 34, 27, 36, 30,
        23,   5, 16,  1, 14,  9, 18,  7,
        12,  3
    };

    /* n >= 1, compute zero‑based index in the period */
    idx_t idx = (n - 1) % 18;               /* 0 <= idx < 18 */

    return (seq_t)wheel_map[idx];
}
