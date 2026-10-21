/*
 * A284260  Greatest prime dividing n which is less than A020639(n)^2, where
 *          A020639(n) is the smallest prime dividing n, a(1) = 1.
 * https://oeis.org/A284260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{p | p divides n, p < (sp(n))^2}, a(1)=1 where sp(n)
 *            is the smallest prime factor of n
 * Range:     u8 n <= 256, u16 n <= 9506, u32 n <= 9506, u64 n <= 9506,
 *            u128 n <= 9506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Greatest prime divisor of n that is less than the square of its
   smallest prime factor. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t N = (uint64_t)n;
    uint64_t sp = 0;
    uint64_t tmp = N;

    /* Find the smallest prime factor sp of N. */
    if ((tmp & 1ULL) == 0) {
        sp = 2;                /* N even */
    } else {
        for (uint64_t i = 3; i <= tmp / i; i += 2) {
            if (tmp % i == 0) {
                sp = i;
                break;
            }
        }
        if (sp == 0) sp = tmp; /* N is prime */
    }

    seq_t bound = (seq_t)sp * (seq_t)sp;   /* (smallest prime factor)^2 */

    /* Search all distinct prime divisors of N and keep the largest
       one that is strictly less than bound. */
    uint64_t best = 0;
    tmp = N;

    /* Handle factor 2 separately. */
    if (tmp % 2 == 0) {
        uint64_t cand = 2;
        if ((seq_t)cand < bound) best = cand;
        while (tmp % 2 == 0) tmp /= 2;
    }

    /* Handle odd factors. */
    for (uint64_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            uint64_t cand = d;
            if ((seq_t)cand < bound && cand > best) best = cand;
            while (tmp % d == 0) tmp /= d;
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1) {
        uint64_t cand = tmp;
        if ((seq_t)cand < bound && cand > best) best = cand;
    }

    /* For safety, fall back to sp (always divides N). */
    if (best == 0) best = sp;

    return (seq_t)best;
}
