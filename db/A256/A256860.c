/*
 * A256860  a(n) = n*(n + 1)*(n + 2)*(n + 3)*(n^2 - n + 5)/120.
 * https://oeis.org/A256860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n^2 - n + 5)/120
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t cancel_factor(seq_t f, unsigned *div)
{
    while (*div % 2 == 0 && f % 2 == 0) {
        f /= 2;
        *div /= 2;
    }
    while (*div % 3 == 0 && f % 3 == 0) {
        f /= 3;
        *div /= 3;
    }
    while (*div % 5 == 0 && f % 5 == 0) {
        f /= 5;
        *div /= 5;
    }
    return f;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t s   = (seq_t)n;
    seq_t f0  = s;
    seq_t f1  = s + 1;
    seq_t f2  = s + 2;
    seq_t f3  = s + 3;
    seq_t f4  = s * s - s + (seq_t)5;

    unsigned div = 120;

    f0 = cancel_factor(f0, &div);
    f1 = cancel_factor(f1, &div);
    f2 = cancel_factor(f2, &div);
    f3 = cancel_factor(f3, &div);
    f4 = cancel_factor(f4, &div);

    seq_t result = f0 * f1 * f2 * f3 * f4;

    if (div > 1)
        result /= (seq_t)div;

    return result;
}
