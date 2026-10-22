/*
 * A391661  Square array read by antidiagonals: T(n,k) is the Kruskal-Macaulay
 *          function L_n of degree n evaluated at k.
 * https://oeis.org/A391661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the antidiagonal read of the square
 *            array * T(n,k) where T(n,k) = max{ m : L_{n+1}(m) <= k }. *
 *            L_d(m) is the Kruskal–Macaulay function of degree d, defined by
 *            * write m = Σ_i C(x_i , d) with x_d > x_{d-1} > … > x_1 >= 0 *
 *            then L_d(m) = Σ_i C(x_i , d-1). * * The functio...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/* Multiplicative binomial coefficient C(n,k) for seq_t values.
 * Assumes 0 <= k <= n. The result fits in seq_t.           */
static inline seq_t binom_seq_t(seq_t n, idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (n < k)  return (seq_t)0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - k + i) / i; /* division is exact */
    return res;
}

/*------------------------------------------------------------*/
/* Kruskal‑Macaulay function L_d(m) (d >= 1, m >= 0).         */
static inline seq_t L_d(idx_t d, seq_t m)
{
    seq_t remaining = m;
    seq_t result    = 0;
    seq_t last_a    = 0;   /* last chosen x_i; 0 means none yet */

    for (int t = (int)d; t >= 1 && remaining > 0; --t)
    {
        /* Find an upper bound for a */
        seq_t hi = 1;
        while (binom_seq_t(hi, t) <= remaining)
            hi <<= 1;          /* double until > remaining */

        /* Ensure a < last_a (strictly decreasing sequence) */
        if (last_a > 0 && hi > last_a - 1)
            hi = last_a - 1;
        if (hi < 1) break;     /* no admissible a */

        /* Binary search the largest a in [1, hi] with C(a,t)<=remaining */
        seq_t low  = 1;
        seq_t high = hi;
        seq_t best = 0;
        while (low <= high)
        {
            seq_t mid = (low + high) / 2;
            seq_t val = binom_seq_t(mid, t);
            if (val <= remaining)
            {
                best = mid;
                low  = mid + 1;
            }
            else
                high = mid - 1;
        }
        if (best == 0) break;            /* cannot pick a term */

        seq_t val = binom_seq_t(best, t);
        remaining -= val;
        result += binom_seq_t(best, t - 1);
        last_a   = best;
    }
    return result;
}

/*------------------------------------------------------------*/
/* T(n,k) = max{ m : L_{n+1}(m) <= k }   (n>=1, k>=0) */
static inline seq_t T(idx_t n, idx_t k)
{
    idx_t d = n + 1;            /* degree in L_d */

    seq_t low  = 0;
    seq_t high = 1;             /* high will be doubled until the condition fails */

    while (L_d(d, high) <= (seq_t)k)
        high <<= 1;             /* high *= 2 */

    /* Binary search for the largest m with L_d(m) <= k */
    while (low < high)
    {
        seq_t mid = low + (high - low + 1) / 2;
        if (L_d(d, mid) <= (seq_t)k)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*------------------------------------------------------------*/
/* Main routine: convert one‑based index n to the antidiagonal
 * position (i,j) and return the corresponding T(i,j).      */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* defined only for n >= 1 */

    /* Find diagonal s such that  s*(s+1)/2 >= n.  (s>=1) */
    idx_t low_s  = 1;
    idx_t high_s = n;               /* upper bound: any term lies on a
                                       diagonal <= n (since each diagonal
                                       contributes at least one element). */
    while (low_s < high_s)
    {
        idx_t mid = low_s + (high_s - low_s) / 2;
        idx_t cum = mid * (mid + 1) / 2;
        if (cum >= n)
            high_s = mid;
        else
            low_s = mid + 1;
    }
    idx_t s = low_s;                /* diagonal index (1‑based) */

    /* Elements before this diagonal: t = (s-1)*s/2 */
    idx_t t   = (s - 1) * s / 2;
    idx_t off = n - t - 1;          /* zero‑based offset within the diagonal */

    idx_t i = off + 1;              /* row index (>=1)          */
    idx_t j = s - i;                /* column index (>=0)       */

    return T(i, j);
}
