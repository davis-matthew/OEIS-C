/*
 * A253368  a(n) = F(12*n)/(12^2) with the Fibonacci numbers F = A000045.
 * https://oeis.org/A253368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(12*n)/144 (OEIS A253368) Recurrence (n ≥ 3): a(n) =
 *            322*a(n-1) - a(n-2), with initial values a(1)=1, a(2)=322
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* safe for n < 1, though unused */
        return 0;
    if (n == 1)
        return 1;
    if (n == 2)
        return 322;

    seq_t prev2 = 1;  /* a(1) */
    seq_t prev1 = 322;/* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        /* The recurrence is safe for arbitrary-precision seq_t; for 
           native integer types it may overflow, but this is acceptable 
           because values larger than the type cannot be represented. */
        cur = prev1 * 322 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
