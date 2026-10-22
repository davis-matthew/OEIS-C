/*
 * A341414  a(n) = (Fibonacci(n)*Lucas(n)) mod 10.
 * https://oeis.org/A341414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2*n) mod 10
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (Fibonacci(n) * Lucas(n)) mod 10 = Fib(2*n) mod 10 */
static const seq_t fib_mod10[60] = {
    0,1,1,2,3,5,8,3,1,4,5,9,4,3,7,0,7,7,4,1,5,6,1,7,8,5,3,8,1,9,
    0,9,9,8,7,5,2,7,9,6,5,1,6,7,3,0,3,3,6,9,5,4,9,3,2,5,7,2,9,1
};

seq_t a(idx_t n)
{
    /* Compute (2*n) mod 60 without overflow: 2*n = 2*(n mod 30) */
    idx_t idx = (n % 30) * 2;
    return fib_mod10[idx];
}
