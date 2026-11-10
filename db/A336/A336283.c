/*
 * A336283  Row sums of A192933.
 * https://oeis.org/A336283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*A001003(n-1) - 2^(n-1)*A001003(n-2) for n >= 3, a(1)
 *            = 1, a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A001003_a(idx_t n) {
    seq_t x2 = 1; /* a(i-2) */
    seq_t x1 = 1; /* a(i-1) */
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        idx_t d = i + 1;
        seq_t q1 = x1 / (seq_t)d;
        seq_t q2 = x2 / (seq_t)d;
        long long r1 = (long long)(x1 % (seq_t)d);
        long long r2 = (long long)(x2 % (seq_t)d);
        long long t = (long long)(6 * i - 3) * r1 - (long long)(i - 2) * r2;
        long long tq = t / (long long)d; /* exact */
        seq_t v = q1 * (seq_t)(6 * i - 3);
        if (tq > 0) v += (seq_t)(unsigned long long)tq;
        v -= q2 * (seq_t)(i - 2);
        if (tq < 0) v -= (seq_t)(unsigned long long)(-tq);
        x2 = x1;
        x1 = v;
    }
    return x1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    seq_t s1 = A001003_a(n - 1);
    seq_t s2 = A001003_a(n - 2);
    seq_t v = s1 + s1 - s2;   /* 2*S(n-1) - S(n-2) */
    idx_t i;
    for (i = 1; i < n; i++) v = v * (seq_t)2;   /* times 2^(n-1) */
    return v;
}
