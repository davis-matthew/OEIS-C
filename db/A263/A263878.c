/*
 * A263878  a(n) = Sum_{k=0..n} (-1)^k*k*Fibonacci(k), where Fibonacci(k) =
 *          A000045(k).
 * https://oeis.org/A263878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * (F(n-3) + n * F(n-1)) - 2 (for n ≥ 3) Initial
 *            values: a(0)=0, a(1)=-1, a(2)=1 Here F(m) is Fibonacci(m) with
 *            F(0)=0, F(1)=1. The Fibonacci values are computed by fast
 *            doubling in O(log n) time; no external cache is required
 * Range:     i8 n <= 8, i16 n <= 18, i32 n <= 39, i64 n <= 84, i128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {                     /* base case: F(0)=0, F(1)=1 */
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t fk, fk1;
    fib_pair(n >> 1, &fk, &fk1);       /* recursive step: compute F(k), F(k+1) */

    /* c = F(2k)   = F(k) * (2*F(k+1) – F(k))   */
    seq_t two_fk1 = fk1 + fk1;
    seq_t c = fk * (two_fk1 - fk);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2   */
    seq_t d = fk * fk + fk1 * fk1;

    if ((n & 1) == 0) {                /* n is even */
        *fn = c;
        *fn1 = d;
    } else {                           /* n is odd */
        *fn = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* explicit small‑n values (definition by sum) */
    if (n == 0) return 0;
    if (n == 1) return -1;
    if (n == 2) return 1;

    /* For n ≥ 3 use the closed formula */
    seq_t fn3, fn1, dummy;
    fib_pair(n - 3, &fn3, &dummy);     /* F(n-3)  */
    fib_pair(n - 1, &fn1, &dummy);     /* F(n-1)  */

    seq_t sign = (n & 1) ? -1 : 1;
    seq_t term = fn3 + ((seq_t) n * fn1);
    return sign * term - 2;
}
