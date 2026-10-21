/*
 * A289251  Triangle T(n, k), n > 0 and 0 <= k < n, read by rows; if gcd(n, k)
 *          = 1, then T(n, k) = modular inverse of k (mod n), otherwise T(n,
 *          k) = k.
 * https://oeis.org/A289251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) row‑wise where
 *            T(n,k) = k if gcd(n,k)>1 else the modular inverse of k (mod n)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for 64‑bit unsigned integers */
static unsigned long long gcd_u64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Extended Euclidean algorithm: modular inverse of a modulo mod (mod > 1) */
static unsigned long long modinv_u64(unsigned long long a, unsigned long long mod)
{
    /* Use signed 128‑bit arithmetic to avoid overflow on intermediate products */
    long long r0 = (long long)a, r1 = (long long)mod;
    long long t0 = 1, t1 = 0;
    while (r1 != 0) {
        long long q = r0 / r1;
        long long r2 = r0 - q * r1;
        r0 = r1; r1 = r2;
        long long t2 = t0 - q * t1;
        t0 = t1; t1 = t2;
    }
    long long inv = t0 % (long long)mod;
    if (inv < 0) inv += mod;
    return (unsigned long long)inv;
}

/* Return the n-th term of the flattened triangle */
seq_t a(idx_t n)
{
    /* n ≥ 1 */
    if (n == 0) return (seq_t)0;       /* undefined for offset 0, return 0 */

    /* Find row r such that r*(r+1)/2 ≥ n, using binary search. */
    unsigned long long lo = 0, hi = (1ULL << 33);   /* hi > maximum possible r */
    while (lo < hi) {
        unsigned long long mid = (lo + hi) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n) hi = mid;
        else          lo = mid + 1;
    }
    unsigned long long r = lo;          /* r ≥ 1 */

    /* Index k (0‑based) within this row */
    __uint128_t prev = (__uint128_t)(r - 1) * r / 2;   /* S(r-1) */
    unsigned long long k = n - (unsigned long long)prev - 1; /* 0 ≤ k < r */

    /* Special case T(r,0) = 0 */
    if (k == 0) return (seq_t)0;

    /* If gcd(r,k) == 1, return modular inverse of k mod r; otherwise k */
    if (gcd_u64(r, k) == 1)
        return (seq_t)modinv_u64(k, r);
    else
        return (seq_t)k;
}
