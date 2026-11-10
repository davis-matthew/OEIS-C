/*
 * A351785  Symmetric array T(n, k), n, k >= 0, read by antidiagonals; for any
 *          m >= 0 with binary expansion Sum_{i >= 0} b_i*2^i, let d(m) =
 *          Sum_{i >= 0} b_i * 2^A130472(i); let t be the inverse of d; T(n,
 *          k) = t(d(n) + d(k)).
 * https://oeis.org/A351785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(r, s-r) with s(s+1)/2 <= n < (s+1)(s+2)/2, r = n -
 *            s(s+1)/2; T(n,k) = t(d(n)+d(k)), where d(m) = Sum
 *            b_i*2^A130472(i), t = inverse of d (computed with a bit array of
 *            dyadic exponents)
 * Range:     u8 n <= 3741, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 94 terms through n = 9869 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A351785_W 90
#define A351785_OFF 45

static void A351785_load(idx_t m, int *arr)
{
    int i;
    for (i = 0; i < A351785_W; i++) arr[i] = 0;
    for (i = 0; i < 64; i++) {
        if ((m >> i) & 1) {
            int e = (i % 2 == 0) ? i / 2 : -((i + 1) / 2);
            arr[e + A351785_OFF] = 1;
        }
    }
}

seq_t a(idx_t n)
{
    /* find s: largest with s(s+1)/2 <= n */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 s = 0;
    {
        /* binary search */
        unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 33;
        while (lo < hi) {
            unsigned __int128 mid = (lo + hi + 1) / 2;
            if (mid * (mid + 1) / 2 <= N) lo = mid; else hi = mid - 1;
        }
        s = lo;
    }
    idx_t r = (idx_t)(N - s * (s + 1) / 2);
    idx_t c = (idx_t)s - r;

    int x[A351785_W], y[A351785_W];
    A351785_load(r, x);
    A351785_load(c, y);

    int carry = 0, p;
    int z[A351785_W];
    for (p = 0; p < A351785_W; p++) {
        int e = x[p] + y[p] + carry;
        z[p] = e & 1;
        carry = e >> 1;
    }

    seq_t result = 0;
    seq_t pw = 1;
    int i;
    for (i = 0; i < 2 * A351785_OFF; i++) {
        int e = (i % 2 == 0) ? i / 2 : -((i + 1) / 2);
        int pos = e + A351785_OFF;
        if (pos >= 0 && pos < A351785_W && z[pos]) result += pw;
        pw *= 2;
    }
    return result;
}
