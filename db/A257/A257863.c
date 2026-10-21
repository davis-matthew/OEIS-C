/*
 * A257863  Expansion of 1/(1 - x - x^2 + x^5 - x^6).
 * https://oeis.org/A257863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-5)+a(n-6) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=7 This implementation uses
 *            a sliding‑window recurrence and computes a(n) in O(n) time with
 *            constant extra memory
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases – values for indices 0 … 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)7;

    /* sliding window storing the last 6 values:
       buf[0] = a(i-6), … , buf[5] = a(i-1)  before the iteration
    */
    seq_t buf[6] = {(seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3,
                    (seq_t)5, (seq_t)7};

    /* compute successive terms up to a(n) */
    for (idx_t i = 6; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1)+a(i-2)-a(i-5)+a(i-6) */
        seq_t next = buf[5] + buf[4] - buf[1] + buf[0];

        /* shift window: drop a(i-6) and add the new value */
        for (int j = 0; j < 5; ++j)
            buf[j] = buf[j + 1];
        buf[5] = next;
    }
    return buf[5];
}
