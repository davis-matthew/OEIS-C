/*
 * A385714  a(n) is the smallest positive integer k such that the Diophantine
 *          equation x^3 + y^3 + z^3 = k^n, where 0 < x < y < z has an integer
 *          solution, or -1 if no such integer exists.
 * https://oeis.org/A385714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A385714: piecewise definition of the smallest k for the equation
   x^3 + y^3 + z^3 = k^n with 0 < x < y < z.
   For n >= 1:
     n = 1 -> 36
     n = 2 -> 6
     n = 3 -> 6
     n = 4 -> 11
     n = 5 -> 6
     n = 6 -> 3
     n >= 23 and n mod 3 = 2 -> 3
     n >= 24 and n mod 3 = 0 -> 2
     n mod 3 = 1 -> 6
     n mod 3 = 2 (and n < 23) -> 6
     n mod 3 = 0 (and 7 <= n < 24) -> 3
   Otherwise return -1. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)36;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)11;
    if (n == 5) return (seq_t)6;
    if (n == 6) return (seq_t)3;
    if (n >= 23 && (n % 3) == 2) return (seq_t)3;
    if (n >= 24 && (n % 3) == 0) return (seq_t)2;
    if ((n % 3) == 1) return (seq_t)6;
    if ((n % 3) == 2) return (seq_t)6;  /* n < 23 case */
    if ((n % 3) == 0) return (seq_t)3;  /* 7 <= n < 24 case */
    return (seq_t)-1; /* no solution found, should not happen */
}
