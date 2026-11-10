/*
 * A347687  Triangle read by rows: T(n,k) (1<=k<=n) = (r(n+k)-r(k))/(2*n),
 *          where {r(i), i>=1} is the n-th row of array A347684.
 * https://oeis.org/A347687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (A347684(n,k+n) - A347684(n,k)) / (2*n), 1<=k<=n, read
 *            by rows; A347684(x,y) = x*red_inv(x,y)+y*red_inv(y,x) if
 *            gcd(x,y)=1 else 0
 * Range:     u8 n <= 88, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 107 terms through n = 4523 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A347687_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static seq_t A347687_modinv(seq_t u, seq_t v)
{
    seq_t a = u % v;
    seq_t b = v;
    seq_t x0 = (seq_t)1;
    seq_t x1 = (seq_t)0;
    seq_t ov = v;
    while (b != (seq_t)0) {
        seq_t q = a / b;
        seq_t r = a % b;
        seq_t temp = (q * x1) % ov;
        seq_t nx = (x0 + ov - temp) % ov;
        a = b;
        b = r;
        x0 = x1;
        x1 = nx;
    }
    return (a == (seq_t)1) ? (x0 % ov) : (seq_t)0;
}

static seq_t A347687_redinv(seq_t u, seq_t v)
{
    if (v == (seq_t)0) return (seq_t)0;
    seq_t inv = A347687_modinv(u, v);
    if (inv == (seq_t)0) return (seq_t)0;
    seq_t half = v >> 1;
    return (inv <= half) ? inv : (v - inv);
}

static seq_t A347687_f(seq_t x, seq_t y)
{
    if (A347687_gcd(x, y) != (seq_t)1) return (seq_t)0;
    return x * A347687_redinv(x, y) + y * A347687_redinv(y, x);
}

seq_t a(idx_t n)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) r++;
    idx_t k = n - (r - 1) * r / 2;
    seq_t R = (seq_t)r;
    seq_t K = (seq_t)k;
    seq_t hi = A347687_f(R, K + R);
    seq_t lo = A347687_f(R, K);
    if (hi < lo) return (seq_t)0;
    return (hi - lo) / ((seq_t)2 * R);
}
