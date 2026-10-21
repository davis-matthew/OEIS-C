/*
 * A277516  a(n) = smallest k >= 0 for which there is a sequence n = b_1 < b_2
 *          < ... < b_t = n + k such that b_1 + b_2 +...+ b_t is a perfect
 *          square.
 * https://oeis.org/A277516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 618, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   ceil_div_seq, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = smallest k ≥ 0 such that there exists a strictly
   increasing sequence n = b1 < b2 < ... < bt = n+k with
   b1+...+bt a perfect square.
   For n a square, a(n)=0.
   The algorithm iterates over possible lengths t≥2, computes the
   smallest square S≥t·n + (t−1)t/2, and derives the minimal k for
   that t; the minimal k over all t is returned.  The running time is
   O(k) per call (k is typically O(√n)).  */

#include "../utils/floor_sqrt.h"
#include "../utils/ceil_div_seq.h"

seq_t a(idx_t n)
{
    seq_t nst = (seq_t)n;

    /* n itself a perfect square -> k = 0 */
    seq_t rt = floor_sqrt(nst);
    if (rt * rt == nst)
        return 0;

    /* Compute best_k using t = 2 as an initial upper bound. */
    idx_t t = 2;
    seq_t tst = (seq_t)t;
    seq_t B_min = (seq_t)(t - 1) * (seq_t)t / 2;       /* (t-1)t/2 */
    seq_t L = tst * nst + B_min;
    seq_t r = floor_sqrt(L);
    if (r * r < L)
        ++r;
    seq_t S = r * r;
    seq_t B = S - tst * nst;
    seq_t delta = B - B_min;
    seq_t divisor = (seq_t)(t - 1);                   /* t-1 */
    seq_t best_k = divisor + ceil_div_seq(delta, divisor);

    /* Try increasing t, stopping when t-1 > current best_k. */
    for (t = 3;; ++t)
    {
        if ((seq_t)(t - 1) > best_k)
            break;

        tst      = (seq_t)t;
        B_min   = (seq_t)(t - 1) * (seq_t)t / 2;       /* (t-1)t/2 */
        L        = tst * nst + B_min;
        r        = floor_sqrt(L);
        if (r * r < L)
            ++r;
        S  = r * r;
        B  = S - tst * nst;
        delta = B - B_min;
        divisor = (seq_t)(t - 1);
        seq_t k_t = divisor + ceil_div_seq(delta, divisor);

        if (k_t < best_k)
            best_k = k_t;
    }

    return best_k;
}
