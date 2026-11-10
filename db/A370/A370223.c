/*
 * A370223  AGM transform of the positive Fibonacci numbers.
 * https://oeis.org/A370223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000071(n+2)^n - A000312(n)*A003266(n), where A000071(m)
 *            = Fibonacci(m) - 1, A000312(n) = n^n, A003266(n) =
 *            Product_{i=1..n} Fibonacci(i)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 67 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A000071_a(idx_t n)
{
    seq_t f0 = 0, f1 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    return f0 - 1;
}

static seq_t A000312_a(idx_t n)
{
    seq_t r = 1;
    seq_t b = (seq_t)n;
    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            r *= b;
        e >>= 1;
        if (e > 0)
            b *= b;
    }
    return r;
}

static seq_t A003266_a(idx_t n)
{
    if (n == 0) return 1;
    seq_t product = 1;
    seq_t fib_prev = 0, fib_curr = 1;
    for (idx_t i = 1; i <= n; i++) {
        product *= fib_curr;
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
    }
    return product;
}

static seq_t A370223_pow(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e > 0) {
        if (e & 1)
            r *= b;
        e >>= 1;
        if (e > 0)
            b *= b;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t base = A000071_a(n + 2);
    seq_t t1 = A370223_pow(base, n);
    seq_t t2 = A000312_a(n) * A003266_a(n);
    return t1 - t2;
}
