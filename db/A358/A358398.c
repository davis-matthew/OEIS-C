/*
 * A358398  a(n) is the number of reducible monic cubic polynomials x^3 +
 *          r*x^2 + s*x + t with integer coefficients bounded by naïve height
 *          n (abs(r), abs(s), abs(t) <= n).
 * https://oeis.org/A358398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reducible monic cubics x^3 + r x^2 + s x + t
 *            with integer coefficients |r|,|s|,|t| ≤ n. This is computed by
 *            inclusion–exclusion over the integer roots a of the cubic
 * Range:     u8 n <= 4, u16 n <= 67, u32 n <= 1250, u64 n <= 1250,
 *            u128 n <= 1250, big any n
 * Helpers:   count_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_root.h"
/* integer floor division, truncating toward -∞ */
static inline int64_t floor_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) != (b > 0))) q--;
    return q;
}

/* integer ceil division, truncating toward +∞ */
static inline int64_t ceil_div(int64_t a, int64_t b) {
    int64_t q = a / b;
    int64_t r = a % b;
    if ((r != 0) && ((r > 0) == (b > 0))) q++;
    return q;
}

seq_t a(idx_t n) {
    int64_t N = (int64_t)n;

    /* Single root contributions */
    seq_t single = count_root(n, 0);
    for (int64_t a = 1; a <= N; ++a) {
        seq_t v = count_root(n, a);
        single += v << 1;          /* multiply by 2 for ±a */
    }

    /* Pair intersections and ordered triples */
    seq_t pair_cnt  = 0;
    seq_t triple_cnt = 0;

    for (int64_t a = -N; a <= N; ++a) {
        if (a == 0) {                         /* a = 0 : pair with b > 0 only */
            for (int64_t b = 1; b <= N; ++b) {

                int64_t ab = 0; /* a*b = 0 */

                /* sum1 bounds */
                int64_t low  = -N - a - b;
                int64_t high =  N - a - b;

                /* sum2 bounds (S = a+b) */
                int64_t S = a + b;              /* always > 0 */
                if (S != 0) {
                    int64_t l2 = ceil_div(-N - ab, S);
                    int64_t r2 = floor_div( N - ab, S);
                    if (l2 > low) low = l2;
                    if (r2 < high) high = r2;
                }
                /* product bound: none (ab = 0) */

                if (low > high) continue;
                pair_cnt += (seq_t)(high - low + 1);

                int64_t effective_low = low;
                if (effective_low < b + 1) effective_low = b + 1;
                if (high >= effective_low)
                    triple_cnt += (seq_t)(high - effective_low + 1);
            }
        } else {                               /* a ≠ 0 */
            int64_t abs_a = a > 0 ? a : -a;
            int64_t bound = N / abs_a;         /* |b| ≤ bound to keep |a*b| ≤ N */

            /* Determine the valid range of b with a < b */
            int64_t b_start = a + 1;
            int64_t b_end   = N;

            if (a > 0) {          /* a positive: b must be ≤ bound */
                if (b_start > bound) continue;
                if (b_end > bound) b_end = bound;
            } else {              /* a negative: |b| ≤ bound */
                if (b_end < -bound) continue;
                if (b_start < -bound) b_start = -bound;
                if (b_end   >  bound) b_end   =  bound;
                if (b_start > b_end)   continue;
            }

            for (int64_t b = b_start; b <= b_end; ++b) {
                int64_t ab = a * b;
                /* |ab| ≤ N by construction */

                /* sum1 bounds */
                int64_t low  = -N - a - b;
                int64_t high =  N - a - b;

                /* sum2 bounds (S = a+b) */
                int64_t S = a + b;
                if (S != 0) {
                    int64_t l2, r2;
                    if (S > 0) {
                        l2 = ceil_div(-N - ab, S);
                        r2 = floor_div( N - ab, S);
                    } else { /* S < 0 */
                        l2 = ceil_div( N - ab, S);
                        r2 = floor_div(-N - ab, S);
                    }
                    if (l2 > low)  low = l2;
                    if (r2 < high) high = r2;
                }
                /* product bound for c */
                if (ab != 0) {
                    int64_t pbound = N / llabs(ab);
                    if (-pbound > low) low = -pbound;
                    if ( pbound < high) high =  pbound;
                }

                if (low > high) continue;
                pair_cnt += (seq_t)(high - low + 1);

                int64_t c_low = low;
                if (c_low < b + 1) c_low = b + 1;
                if (high >= c_low)
                    triple_cnt += (seq_t)(high - c_low + 1);
            }
        }
    }

    return single - pair_cnt + triple_cnt;
}
