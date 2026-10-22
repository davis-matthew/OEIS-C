/*
 * A347683  Triangle read by rows: T(n,k) (1<=k<=n) = f(n,k), where f(x,y) =
 *          x*red_inv(x,y) + y*red_inv(y,x) if gcd(x,y)=1, or 0 if gcd(x,y)>1,
 *          and red_inv is defined in the comments.
 * https://oeis.org/A347683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(row(n),col(n)) * where f(x,y) = 0 if gcd(x,y) > 1 *
 *            otherwise f(x,y) = x*red_inv(x,y) + y*red_inv(y,x) * and
 *            red_inv(u,v) is the reduced inverse of u modulo v: * let inv =
 *            u^{-1} mod v * if inv <= v/2 then inv else v - inv
 * Range:     u8 n <= 187, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Extended Euclidean algorithm for signed integers. */
static void egcd(int64_t a, int64_t b, int64_t *x, int64_t *y)
{
    int64_t old_r = a, r = b;
    int64_t old_s = 1, s = 0;
    int64_t old_t = 0, t = 1;
    while (r != 0) {
        int64_t q = old_r / r;
        int64_t tmp;

        tmp = old_r - q * r; old_r = r; r = tmp;
        tmp = old_s - q * s; old_s = s; s = tmp;
        tmp = old_t - q * t; old_t = t; t = tmp;
    }
    *x = old_s;
    *y = old_t;
}

/* Compute modular inverse of u modulo v (v>1), assuming gcd(u,v)=1. */
static idx_t mod_inverse(idx_t u, idx_t v)
{
    int64_t x, y;
    egcd((int64_t)u, (int64_t)v, &x, &y);
    /* make x positive modulo v */
    int64_t result = x % (int64_t)v;
    if (result < 0) result += (int64_t)v;
    return (idx_t)result;   /* 1 <= result < v */
}

/* reduced inverse according to the OEIS definition */
static uint64_t red_inv(idx_t u, idx_t v)
{
    idx_t inv = mod_inverse(u, v);          /* 1 <= inv < v */
    if ((uint64_t)inv * 2 <= (uint64_t)v)   /* inv <= v/2 */
        return (uint64_t)inv;
    else
        return (uint64_t)v - (uint64_t)inv;
}

/* Compute f(x,y) as defined in the sequence */
static seq_t f(idx_t x, idx_t y)
{
    if (gcd((seq_t)x, (seq_t)y) != (seq_t)1) return (seq_t)0;
    uint64_t rxy = red_inv(x, y);
    uint64_t ryx = red_inv(y, x);
    seq_t val = (seq_t)x * (seq_t)rxy + (seq_t)y * (seq_t)ryx;
    return val;
}

/* find the row r such that (r*(r+1))/2 >= n */
static idx_t row_from_n(idx_t n)
{
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) ++r;
    return r;
}

/* the main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* offset 1; behaviour below is irrelevant */

    idx_t r = row_from_n(n);
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t c = n - prev_tri;                /* column in this row */

    return f(r, c);
}
