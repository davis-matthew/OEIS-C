/*
 * A278716  Triangle read by rows: T(k, h) gives the denominators of the
 *          Dedekind sums s(h, k) with gcd(h, k) = 1 or 0 otherwise.
 * https://oeis.org/A278716
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = denominator of the Dedekind sum * s(h,k) = Σ_{t=1}^{k-1}
 *            ((t/k)) ⋅ ((h·t/k)), * where ((x)) = x − ⌊x⌋ − ½. * The linear
 *            index n≥2 is mapped to the triangle entry * T(k,h) (G.H. G.N.
 *            and h in 1,…,k−1, gcd(h,k)=1; * otherwise the entry is 1). * *
 *            For k≥2 the number of entries up to row k is...
 * Range:     u8 n <= 1082, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   _gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gcd_u64.h"

/*------------------------------------------------------------*/
/*  GCD for 64‑bit unsigned integers (used for small k,h).   */

/*  GCD for unsigned long long, used for the denominator
 *   (denominations are below 2^32 in the range of the sequence). */
static inline uint64_t _gcd_ull(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /*  The sequence is defined for n ≥ 2. */
    if (n < 2) return 0;

    /*  Linear index starting from 0 for the first entry (k=2,h=1). */
    idx_t idx = n - 2;

    /*  Determine k by finding the largest k with (k−1)k/2 ≤ idx. */
    idx_t k = 2;
    while ((k * (k - 1) / 2) <= idx) k++;

    /*  Entries preceding row k:  (k−2)(k−1)/2. */
    idx_t prev = (k - 2) * (k - 1) / 2;
    idx_t h = idx - prev + 1;      /* 1 ≤ h ≤ k−1 */

    /*  If gcd(h,k)>1 the Dedekind sum is 0 → denominator = 1. */
    if (_gcd_u64((uint64_t)h, (uint64_t)k) != 1) return (seq_t)1;

    /*  Compute the numerator of s(h,k) as described:
     *      numerator = Σ_{t=1}^{k-1} (2t−k)(2m−k),
     *      where m = (h·t) mod k. */
    int64_t num = 0;
    for (idx_t t = 1; t < k; ++t) {
        idx_t m = (h * t) % k;
        int64_t a = (int64_t)(2 * t) - (int64_t)k;
        int64_t b = (int64_t)(2 * m) - (int64_t)k;
        num += a * b;
    }

    /*  Denominator before reduction: 4·k². */
    uint64_t denom = (uint64_t)4 * (uint64_t)k * (uint64_t)k;

    /*  Reduce the fraction. */
    uint64_t abs_num = (num >= 0) ? (uint64_t)num : (uint64_t)(-num);
    uint64_t g = _gcd_ull(abs_num, denom);
    uint64_t denom_reduced = denom / g;

    return (seq_t)denom_reduced;
}
