/*
 * A259688  Related to Euler numbers, expansion of e.g.f. tan(x)^2.
 * https://oeis.org/A259688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = b(2n+3)/2 , a(2n+1) = b(2n+3) where b(k) = A000111(k)
 *            (Euler zig‑zag / tangent numbers). The implementation computes
 *            the required tangent numbers with an integer recurrence, stores
 *            them in a cache, and evaluates a(n) from the cache. All
 *            arithmetic is performed with the seq_t type
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *T = NULL;          /* tangent numbers T_{2k+1},  T[0] = 1 */
static size_t T_size = 0;        /* number of computed tangent numbers */

/* Ensure the tangent array contains entries up to index 'need' (inclusive). */
static void ensure_T_size(size_t need)
{
    if (T_size > need) return;

    /* Grow the array. */
    size_t new_cap = (T_size == 0) ? 4 : T_size * 2;
    while (new_cap <= need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
    if (!tmp) return;           /* allocation failure – abort silently */
    T = tmp;

    /* Initialize first element. */
    if (T_size == 0) {
        T[0] = (seq_t)1;
        T_size = 1;
    }

    /* Compute new tangent numbers up to 'need'. */
    for (size_t i = T_size; i <= need; ++i) {
        if (i == 0) { T[0] = (seq_t)1; continue; }

        /* Recurrence:  T_{2i+1} = Σ_{k=1..i} C(2i, 2k-1) * T_{2k-1} * T_{2i-2k+1}   */
        size_t n = i;
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)2 * (seq_t)n;      /* C(2n,1) */
        unsigned int r = 1;                     /* current odd r = 2k-1 */

        for (size_t k = 1; k <= n; ++k) {
            seq_t term = binom * T[k-1] * T[n-k];
            sum += term;

            if (k < n) {
                /* Update binom to C(2n, r+2) from C(2n, r): */
                seq_t mult1 = (seq_t)(2*n - r);
                seq_t mult2 = (seq_t)(2*n - r - 1);
                seq_t denom = (seq_t)(r+1) * (seq_t)(r+2);
                binom = (binom * mult1 * mult2) / denom;
                r += 2;
            }
        }
        T[i] = sum;
    }
    T_size = need + 1;
}

seq_t a(idx_t n)
{
    /* Even indices: a(2m)  = T_{2m+3} / 2  -> T index = m+1 = n/2 + 1   */
    if ((n & 1ULL) == 0) {
        size_t k = (n / 2) + 1;
        ensure_T_size(k);
        seq_t res = T[k];
        res /= (seq_t)2;
        return res;
    }

    /* Odd indices:  a(2m+1) = T_{2m+3}       -> T index = m+1 = (n+1)/2 */
    size_t k = (n + 1) / 2;
    ensure_T_size(k);
    return T[k];
}
