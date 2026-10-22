/*
 * A377016  Semiperimeter of the unique primitive Pythagorean triple whose
 *          short leg is A002315(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A377016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = x_n*(x_n+1)/2 where * x_0 = 1, x_1 = 7 * x_{k+1} = 6*x_k
 *            - x_{k-1} (x_k are the x‑coordinates of the solutions to x^2 – 2
 *            y^2 = –1) * This yields the semiperimeter of the unique
 *            primitive Pythagorean triple with short leg * x_n and with long
 *            leg + hypotenuse consecutive
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)28;   /* 7*8/2 */

    seq_t x_prev = (seq_t)1;         /* x_0 */
    seq_t x_curr = (seq_t)7;         /* x_1 */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t x_next = (seq_t)6 * x_curr - x_prev;
        x_prev = x_curr;
        x_curr = x_next;
    }
    return x_curr * (x_curr + 1) / 2;
}
