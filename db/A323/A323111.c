/*
 * A323111  Number of compositions of 2n where the difference between largest
 *          and smallest parts equals n.
 * https://oeis.org/A323111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of 2n such that (max part – min
 *            part) = n. Implementation based on divisor enumeration of k·m =
 *            2n – a with inclusion–exclusion on bounded compositions
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t count_minmax(idx_t k, idx_t a, idx_t n)
{
    /* total compositions with parts in [0,n] */
    seq_t total = binom(a + k - 1, k - 1);
    if (a >= n + 1) {
        seq_t sub = (seq_t)k *
                    binom(a - (n + 1) + k - 1, k - 1);
        total -= sub;
    }

    /* compositions with no zero part (all parts ≥1) */
    seq_t no0 = 0;
    if (a >= k) {
        no0 = binom(a - k + k - 1, k - 1);
    }
    if (a >= k + n) {
        seq_t sub = (seq_t)k *
                    binom(a - k - n + k - 1, k - 1);
        no0 -= sub;
    }

    /* compositions with no n part (all parts ≤ n‑1) */
    seq_t noN = 0;
    if (a >= n) {
        noN = binom(a + k - 1, k - 1);
        seq_t sub = (seq_t)k *
                    binom(a - n + k - 1, k - 1);
        noN -= sub;
    } else {
        /* a < n cannot occur for a in [n,2n−2], but kept for safety */
        noN = binom(a + k - 1, k - 1);
    }

    /* compositions with parts in [1,n‑1] (no zeros, no n) */
    seq_t both = 0;
    if (a >= k) {
        both = binom(a - k + k - 1, k - 1);
    }
    if (a >= k + n - 1) {
        seq_t sub = (seq_t)k *
                    binom(a - k - (n - 1) + k - 1, k - 1);
        both -= sub;
    }

    /* f = total − no0 − noN + both */
    seq_t res = total + both;
    res -= no0;
    res -= noN;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(1) = 0 */

    seq_t sum = 0;

    for (idx_t a_val = n; a_val <= 2 * n - 2; ++a_val) {
        idx_t T = 2 * n - a_val;          /* T = k · m */
        /* k = T itself (corresponding to divisor 1) */
        if (T >= 2 && T <= n) {
            sum += count_minmax(T, a_val, n);
        }
        /* other divisors of T (≥2) */
        for (idx_t d = 2; d * d <= T; ++d) {
            if (T % d == 0) {
                idx_t k1 = d;
                if (k1 <= n) {
                    sum += count_minmax(k1, a_val, n);
                }
                idx_t k2 = T / d;
                if (k2 != k1 && k2 <= n) {
                    sum += count_minmax(k2, a_val, n);
                }
            }
        }
    }
    return sum;
}
