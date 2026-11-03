/*
 * A335824  Persistence of the 1-shifted Sloane's problem: number of
 *          iterations of "multiply together all the digits of a number (in
 *          base 10) shifted by +1" needed to reach a fixed point or a cycle.
 * https://oeis.org/A335824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t shifted_product(seq_t x)
{
    seq_t prod = 1;
    if (x == 0)          /* digit 0 -> 1, product 1 */
        return 1;
    while (x != 0) {
        int d = (int)(x % 10);   /* current decimal digit of x */
        prod = prod * (d + 1);   /* multiply by shifted digit */
        x = x / 10;
    }
    return prod;
}
/* Persistence of 1‑shifted Sloane's problem:
   Starting from n, repeatedly apply
   f(x) = product of (digit of x + 1) until x equals
   the fixed point 18 or enters the cycle {2,3,…,10}.
   Return the number of steps taken. */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;   /* start from n */
    seq_t steps = 0;
    /* stop if x is the fixed point 18 or in the cycle 2..10 */
    while (x != 18 && !(x >= 2 && x <= 10)) {
        x = shifted_product(x);
        ++steps;
    }
    return steps;
}
