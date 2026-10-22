/*
 * A389570  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^j *
 *          binomial(k,j) * (1-j)^n.
 * https://oeis.org/A389570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(r,k) where r and k are from the row/column of the
 *            triangle T(n,k) = Σ_{j=0..k} (-1)^j * binomial(k,j) * (1-j)^n
 * Range:     i8 n <= 24, i16 n <= 40, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the sequence obtained by reading the triangle row‑by‑row
   starting with n=0. */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute binomial coefficient C(n,k) as 128‑bit integer. */
static __int128 binom128(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (__int128)(n - k + i) / (__int128)i;
    }
    return res;
}

/* Helper: 128‑bit integer power: base^exp, exp >= 0. */
static __int128 pow128(__int128 base, idx_t exp) {
    __int128 result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Helper: find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_from_index(idx_t n) {
    idx_t low = 0, high = (idx_t)(1024); /* enough for small n */
    /* enlarge high until it exceeds n */
    while ((__int128)high * (high + 1) / 2 <= (__int128)n) high *= 2;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= (__int128)n) {
            if ((__int128)(mid + 1) * (mid + 2) / 2 > (__int128)n)
                return mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return 0;
}

seq_t a(idx_t n) {
    /* locate (r,k) in the triangle */
    idx_t r = row_from_index(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* compute T(r,k) = Σ_{j=0..k} (-1)^j * C(k,j) * (1-j)^r */
    __int128 sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        __int128 bin = binom128(k, j);
        __int128 base = (__int128)1 - (__int128)j;
        __int128 power = pow128(base, r);
        __int128 term = bin * power;
        if (j & 1) term = -term;
        sum += term;
    }

    /* cast result to seq_t (will wrap if out of range) */
    return (seq_t)sum;
}
