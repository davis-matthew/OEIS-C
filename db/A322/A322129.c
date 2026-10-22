/*
 * A322129  Digital roots of A057084.
 * https://oeis.org/A322129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A030132(24 - (n mod 24))
 * Range:     u8 none, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital roots of A057084, equivalent to the reversed digital
 * roots of Fibonacci numbers (period 24). */
seq_t a(idx_t n)
{
    /* n is 1‑based; if called with n == 0, return 0. */
    if (n == 0) return 0;

    /* k = 24 - n mod 24, with k in [1,24] */
    idx_t rem = n % 24;
    idx_t k = (rem == 0) ? 24 : 24 - rem;

    /* Compute the k‑th Fibonacci number (k ≥ 1). */
    seq_t fib  = 1;   /* fib(1) = 1 */
    seq_t prev = 0;   /* fib(0) = 0 */
    for (idx_t i = 1; i < k; ++i) {
        seq_t tmp = fib + prev;
        prev = fib;
        fib  = tmp;
    }

    /* Digital root of fib(k). */
    seq_t dr = fib;
    if (dr == 0) return 0;
    dr %= 9;
    return (dr == 0) ? 9 : dr;
}
