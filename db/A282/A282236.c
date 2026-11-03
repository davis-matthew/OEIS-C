/*
 * A282236  a(n) = Fibonacci(n) represented in bijective base-5 numeration.
 * https://oeis.org/A282236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = bijective base-5 representation of the nth Fibonacci
 *            number (i.e. a(n) = A084545(A000045(n)))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 947 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) using the provided utility */
    seq_t N = fib((idx_t)n);          /* F(n) > 0 for n >= 1 */

    /* Temporary buffer for bijective‑base‑5 digits (least significant first) */
    /* max ~ ceil(log_5(Fib(10000))) ≈ 3000, keep some margin */
    unsigned char digits[8192];
    size_t len = 0;

    while (N != 0)
    {
        seq_t r = N % 5;                 /* 0 … 4 */
        seq_t digit = (r == 0) ? 5 : r;  /* bijective digit 1 … 5 */
        digits[len++] = (unsigned char)digit;
        N = (N - digit) / 5;
    }

    /* Build the decimal number from most significant digit to least */
    seq_t result = 0;
    while (len-- > 0)
    {
        result = result * 10 + digits[len];
    }

    return result;
}
