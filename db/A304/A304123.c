/*
 * A304123  Denominators of records low values of the ratio
 *          n*3^A006667(n)/2^A006666(n).
 * https://oeis.org/A304123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   a(n) = denominator of the n-th strict record low of r(k) =
 *            k*3^A006667(k)/2^A006666(k), in lowest terms; found by a search
 *            over k = 1, 2, 3,
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static int A304123_exps[4096];
static int A304123_cnt = 0;
static uint64_t A304123_next = 1;
static long double A304123_best = 0.0L;

static void A304123_extend(idx_t want) {
    const long double LN2 = 0.69314718055994530941723212145817657L;
    const long double LN3 = 1.09861228866810969139524523692252571L;
    while ((idx_t)A304123_cnt < want && A304123_cnt < 4096) {
        uint64_t k = A304123_next++;
        unsigned __int128 cur = k;
        int t = 0, h = 0;
        while (cur != 1) {
            if ((cur & 1) == 0) { cur >>= 1; h++; }
            else { cur = cur * 3 + 1; t++; }
        }
        long double L = logl((long double)k) + (long double)t * LN3 - (long double)h * LN2;
        int v = 0;
        uint64_t kk = k;
        while ((kk & 1) == 0) { kk >>= 1; v++; }
        if (A304123_cnt == 0) {
            A304123_best = L;
            A304123_exps[A304123_cnt++] = h - v;
        } else if (L < A304123_best - 1e-12L) {
            A304123_best = L;
            A304123_exps[A304123_cnt++] = h - v;
        }
    }
}

seq_t a(idx_t n) {
    A304123_extend(n);
    int e = A304123_exps[n - 1];
    seq_t r = 1;
    for (int i = 0; i < e; i++) r = r * 2;
    return r;
}
