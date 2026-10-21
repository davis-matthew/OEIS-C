/*
 * A287796  Ten steps forward, nine steps back.
 * https://oeis.org/A287796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (-1)^floor((2*i-2)/19) Recurrence for n >
 *            19: a(n) = a(n-1) + a(n-19) - a(n-20) Initial values: a(0)=0;
 *            a(1)=1…a(10)=10; a(11)=9…a(18)=2; a(19)=1
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = (i - 1) % 19 + 1;   /* position inside a 19‑block */
        if (r <= 10)
            ans += 1;
        else
            ans -= 1;
    }
    return ans;
}
