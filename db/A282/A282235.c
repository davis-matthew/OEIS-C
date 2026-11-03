/*
 * A282235  a(n) = Fibonacci(n) represented in bijective base-4 numeration.
 * https://oeis.org/A282235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = bijective base‑4 representation of Fibonacci(n)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the provided fast Fibonacci routine. */
#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Convert the positive integer x to its bijective base‑4
   representation, encoded as a decimal integer whose digits are
   1–4. */
static seq_t _bij4_from_seq(seq_t x)
{
    seq_t result = 0;
    seq_t mult   = 1;

    while (x > 0) {
        /* After decrementing, the digit is x % 4 + 1. */
        seq_t t     = x;
        t--;                       /* x-- */
        seq_t digit = (t % 4) + 1;  /* digit in 1..4 */
        result += digit * mult;
        mult   *= 10;
        x = t / 4;                 /* floor division by 4 */
    }
    return result;
}

/* Public sequence function.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1)          /* behaviour below the offset is irrelevant */
        return 0;
    seq_t f = fib(n);           /* Fibonacci(n) */
    return _bij4_from_seq(f);
}
