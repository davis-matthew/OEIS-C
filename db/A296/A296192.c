/*
 * A296192  Denominators of the Harary index of the n X n grid graph.
 * https://oeis.org/A296192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  A296192 – Denominators of the Harary index of the n×n grid graph.
 *  The Harary index H(G) is the sum over all unordered pairs of vertices
 *  of the reciprocal of their graph distance.
 *  For the n×n grid we compute the rational value of H(G) exactly and
 *  return its denominator in lowest terms.
 *
 *  The implementation uses the closed‑form formula for the number of
 *  unordered vertex pairs at a given distance d (1 ≤ d ≤ 2n–2),
 *  then accumulates the fraction ∑_d cnt[d]/d, simplifying after every
 *  addition by the greatest common divisor.  All arithmetic is performed
 *  with the generic type seq_t, which may be any unsigned integer type
 *  supported by the build (uint8_t … unsigned __int128 or an arbitrary-
 *  precision integral class).
 *
 *  Complexity:  O(n) per call, with only a few big‑integer operations
 *  per iteration.  The algorithm is deterministic and works for any
 *  order of calls.  No memoisation is needed because the cost is small.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /*  denominator of the Harary index for an n×n grid graph  */
    if (n <= 1)  /* a(1) = 1  */
        return (seq_t)1;

    seq_t num = 0;          /* current numerator  */
    seq_t den = 1;          /* current denominator */

    idx_t max_d = 2 * n - 2;   /* largest Manhattan distance */

    for (idx_t d = 1; d <= max_d; ++d)
    {
        int64_t cnt_val;   /* temporary signed intermediate */

        if (d <= n - 1)                /* 1 ≤ d ≤ n-1  */
        {
            int64_t dd = (int64_t)d;
            int64_t term1 = 2 * dd * (int64_t)(n - dd) * (int64_t)n;
            int64_t term3 = dd * (dd - 1) * (dd + 1) / 3;
            cnt_val = term1 + term3;
        }
        else                            /* n ≤ d ≤ 2n-2  */
        {
            int64_t low  = (int64_t)d - (n - 1);
            int64_t high = n - 1;
            int64_t k    = high - low + 1;                    /* 2n-d-1 */

            int64_t sum_dx  = (low + high) * k / 2;
            int64_t high3   = high * (high + 1) * (2 * high + 1);
            int64_t lowm1   = low - 1;
            int64_t low3    = lowm1 * low * (2 * lowm1 + 1);
            int64_t sum_dx2 = (high3 - low3) / 6;

            int64_t sum_t   = (int64_t)(n - d) * n * k + d * sum_dx - sum_dx2;
            cnt_val = 2 * sum_t;              /* all terms have factor 2 */
        }

        seq_t cnt = (seq_t)cnt_val;             /* cast to seq_t */

        /* Add cnt/d to the running fraction num/den */
        seq_t new_num = num * d + cnt * den;
        seq_t new_den = den * d;
        seq_t g = gcd(new_num, new_den);

        num = new_num / g;
        den = new_den / g;
    }

    return den;          /* the denominator in lowest terms */
}
