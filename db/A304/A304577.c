/*
 * A304577  Period 21: repeat (0,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,1,0,0,0,1).
 * https://oeis.org/A304577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the sequence is periodic with period 21. a(n) = a(n-21) for n >=
 *            21. The period pattern is:
 *            0,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,1,0,0,0,1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[21] = {
        0,0,0,0,1,1,1,1,1,0,
        1,0,1,0,0,1,1,0,0,0,1
    };
    return period[n % 21];
}
