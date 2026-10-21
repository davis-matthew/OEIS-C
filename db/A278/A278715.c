/*
 * A278715  Table T read by rows. T(k, h) gives the numerators of the Dedekind
 *          sums s(h, k) with gcd(h, k) = 1 or 0 otherwise.
 * https://oeis.org/A278715
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabl, frac, easy
 * Formula:   a(n) returns the numerator of the Dedekind sum s(h,k) for the
 *            triangular indexing used by OEIS sequence A278715. The value is
 *            0 when gcd(h,k)≠1 or when the sum itself is 0. The
 *            implementation uses integer arithmetic only
 * Range:     i8 n <= 16, i16 n <= 3655, i32 n <= 4951, i64 n <= 4951,
 *            i128 n <= 4951, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* integer square root: floor(sqrt(n)) for n <= 2^64-1 */
static idx_t isqrt_u64(idx_t n)
{
    idx_t root = 0;
    idx_t bit = (idx_t)1ULL << 62;   /* largest power of four ≤ n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= root + bit) {
            n -= root + bit;
            root = (root >> 1) + bit;
        } else {
            root >>= 1;
        }
        bit >>= 2;
    }
    return root;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* offset is 2; values below unused */

    /* Find (k,h) from the triangular numbering */
    idx_t m = n - 2;                         /* 0‑based index */
    idx_t t = isqrt_u64(8 * m + 1);          /* floor(sqrt(8m+1))          */
    idx_t k = (t + 1) / 2 + 1;               /* k ≥ 2                       */
    idx_t sum_prev = (k - 2) * (k - 1) / 2;  /* terms before row k         */
    idx_t h = (m - sum_prev) + 1;           /* 1 ≤ h < k                   */

    seq_t kk = (seq_t)k;
    seq_t hh = (seq_t)h;

    if (gcd(kk, hh) != 1) return 0;         /* non‑coprime entries are 0   */

    /* Compute the Dedekind sum numerator S = Σ (2r-k)*(2*(hr mod k)-k) */
    seq_t S = 0;
    for (idx_t r = 1; r < k; ++r) {
        idx_t mod = (h * r) % k;
        if (mod == 0) continue;             /* sawtooth value is 0         */
        seq_t a_val = (seq_t)(2 * (seq_t)r - (seq_t)k);
        seq_t b_val = (seq_t)(2 * (seq_t)mod - (seq_t)k);
        S += a_val * b_val;
    }
    if (S == 0) return 0;                   /* zero sum yields zero        */

    seq_t denom = (seq_t)4 * kk * kk;        /* common denominator 4k²      */
    seq_t absS   = (S >= 0) ? S : -S;       /* absolute value for GCD      */
    seq_t g      = gcd(absS, denom);        /* reduce the fraction         */
    seq_t numer  = (absS / g);
    numer = (S >= 0) ? numer : -numer;      /* restore sign                */

    return numer;
}
