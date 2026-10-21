/*
 * A274074  a(n) = 6^n+(-1)^n.
 * https://oeis.org/A274074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + (-1)^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) + 6*a(n-2) for n > 1, a(0)=2, a(1)=5 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 6;
    idx_t exp    = n;

    /* binary exponentiation: compute 6^n */
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* add (-1)^n: add 1 if n even, subtract 1 if n odd */
    if (n & 1)
        result--;      /* n is odd */
    else
        result++;      /* n is even */

    return result;
}
