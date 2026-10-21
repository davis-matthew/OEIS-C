/*
 * A264613  Numbers n such that the Shevelev polynomial {m, n} has a root at m
 *          = -1.
 * https://oeis.org/A264613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * if n % 3 == 1: 3*2^(2*((n+2)/3)-2)-1 * if n % 3 == 2:
 *            3*2^(2*((n+2)/3)-1)-1 * if n % 3 == 0: 2^(2*((n+2)/3)+1) * (m =
 *            floor((n+2)/3).)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_two(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result <<= 1;          /* shift by 1 is safe for all unsigned types */
    return result;
}

seq_t a(idx_t n)
{
    idx_t m = (n + 2) / 3;      /* cycle index */
    idx_t cycle_pos = (n - 1) % 3;
    seq_t val = 0;

    if (cycle_pos == 0) {                     /* b_m = 3*2^(2m-2)-1 */
        val = power_of_two(2 * m - 2);
        val = (seq_t)3 * val - (seq_t)1;
    } else if (cycle_pos == 1) {                 /* c_m = 3*2^(2m-1)-1 */
        val = power_of_two(2 * m - 1);
        val = (seq_t)3 * val - (seq_t)1;
    } else {                                      /* X_m = 2^(2m+1) */
        val = power_of_two(2 * m + 1);
    }
    return val;
}
