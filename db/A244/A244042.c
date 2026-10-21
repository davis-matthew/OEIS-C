/*
 * A244042  In ternary representation of n, replace 2's with 0's.
 * https://oeis.org/A244042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   In ternary representation of n, replace 2's with 0's
 * Range:     u8 n <= 255, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    int digits[50];
    int cnt = 0;
    idx_t temp = n;

    /* Extract ternary digits (least‑significant first). */
    while (temp > 0)
    {
        int d = temp % 3;
        temp /= 3;
        digits[cnt++] = d;
    }

    seq_t result = 0;

    /* Build the result from most‑significant to least‑significant digit. */
    for (int i = cnt - 1; i >= 0; --i)
    {
        int d = digits[i];
        if (d == 2) d = 0;
        result = result * 3 + (seq_t)d;
    }

    return result;
}
