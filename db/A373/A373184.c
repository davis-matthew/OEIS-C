/*
 * A373184  G.f. A(x) satisfies A(x) = 1/(1 - x)^2 - 1 + A(x^3).
 * https://oeis.org/A373184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 if n mod 3 != 0 * a(n) = n + 1 + a(n / 3) if n mod
 *            3 == 0 * * This follows from the recurrence * a(3k+1) = 3k+2,
 *            a(3k+2) = 3k+3, * a(3k+3) = 3k+4 + a(k+1) for k >= 0, * which
 *            simplifies to the above piecewise definition. * * The function
 *            works for all n >= 1 and runs in O(log₃ n) time
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;               /* accumulate the contributions of factors of 3 */
    while (n % 3 == 0) {
        sum += (seq_t)n + (seq_t)1;
        n /= 3;
    }
    sum += (seq_t)n + (seq_t)1;   /* final (non‑multiple‑of‑3) part */
    return sum;
}
