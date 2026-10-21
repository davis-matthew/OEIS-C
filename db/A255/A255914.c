/*
 * A255914  Triangle read by rows: T(n,k) = A007318(n,k)*A238453(n,k).
 * https://oeis.org/A255914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  T(n,k) = Π_{i=1..n} A002618(i) / (Π_{i=1..k} A002618(i) · Π_{i=1..n-k} A002618(i))
 *  (Triangle read by rows; offset 0, so a(0)=T(0,0)=1)
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* Special case for the first term */
    if (n == 0) return (seq_t)1;

    /* ---- 1. Locate row r such that n lies in row r ---- */
    /* r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high;
    /* Rough upper bound: sqrt(2*n) + 3  (safe for all uint64_t) */
    {
        long double approx = sqrt((long double)2.0L * (long double)n) + 3.0L;
        high = (idx_t)approx;
        if (high < 1) high = 1;
    }
    /* Ensure high is large enough */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high > n) { high = n; break; }
    }
    /* Binary search */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column k inside row r */
    idx_t prev_cum = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - prev_cum;
    if (k == 0 || k == r) return (seq_t)1;  /* borders are always 1 */

    /* ---- 2. Compute pref[i] = Π_{j=1..i} A002618(j) for i=0..r ---- */
    /* Build smallest‑prime‑factor table up to r */
    idx_t *spf = NULL;
    build_spf(r, &spf);                     /* returns array of size r+1 */
    if (!spf) return (seq_t)0;               /* allocation failure */

    /* Prefix product array */
    seq_t *pref = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!pref) { free(spf); return (seq_t)0; }
    pref[0] = (seq_t)1;

    for (idx_t i = 1; i <= r; ++i) {
        /* Compute φ(i) using the SPF table */
        idx_t x = i;
        idx_t phi_i = x;
        idx_t last_p = 0;
        while (x > 1) {
            idx_t p = spf[x];
            if (p != last_p) {
                phi_i = phi_i / p * (p - 1);
                last_p = p;
            }
            x /= p;
        }

        /* A002618(i) = i * φ(i) */
        seq_t a002618 = (seq_t)i * (seq_t)phi_i;
        pref[i] = pref[i - 1] * a002618;
    }

    /* ---- 3. Evaluate T(r,k) = pref[r] / (pref[k] * pref[r-k]) ---- */
    seq_t denom  = pref[k] * pref[r - k];
    seq_t result = pref[r] / denom;

    free(pref);
    free(spf);
    return result;
}
