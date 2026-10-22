/*
 * A387650  a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(2*k+1,2*n-6*k).
 * https://oeis.org/A387650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^(n-3*k) * binomial(2*k+1,
 *            2*n-6*k)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 91, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* recurrence: a(n) = 2*a(n-3) + 4*a(n-4) - a(n-6) + 4*a(n-7) - 4*a(n-8) */
/* offset 0, base values: a(0)=1,a(1)=0,a(2)=0,a(3)=1,a(4)=6,a(5)=0,a(6)=1,a(7)=20 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {1, 0, 0, 1, 6, 0, 1, 20};

    if (n < 8)
        return init[n];

    seq_t buf[9];
    for (int i = 0; i < 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = 0;
        val += buf[(i - 3) % 9] * (seq_t)2;
        val += buf[(i - 4) % 9] * (seq_t)4;
        val += buf[(i - 7) % 9] * (seq_t)4;
        val -= buf[(i - 6) % 9];
        val -= buf[(i - 8) % 9] * (seq_t)4;
        buf[i % 9] = val;
    }

    return buf[n % 9];
}
