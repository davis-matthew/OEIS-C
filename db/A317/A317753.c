/*
 * A317753  Number of steps to reach 1 in 7x+-1 problem, or -1 if 1 is never
 *          reached.
 * https://oeis.org/A317753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, hear, look
 * Formula:   a(n) = number of steps to reach 1 in the 7x±1 problem (total
 *            stopping time), or -1 if the trajectory never reaches 1
 * Range:     u8 n <= 10, u16 n <= 34, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Leave the fictitious offset 0 undefined. */
    if (n == 0) return (seq_t)-1;

    seq_t steps = 0;
    seq_t m = (seq_t)n;

    while (m != 1)
    {
        /* n % 4 == 1  ⇒  7*n + 1 */
        if ((m & 3) == 1)
            m = 7*m + 1;
        /* n % 4 == 3  ⇒  7*n - 1 */
        else if ((m & 3) == 3)
            m = 7*m - 1;
        /* otherwise divide by 2 */
        else
            m >>= 1;

        ++steps;

        /* Safeguard against non‑terminating cycles. */
        if (steps > 1000000)
            return (seq_t)-1;
    }

    return steps;
}
