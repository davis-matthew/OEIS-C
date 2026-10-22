/*
 * A360124  Numbers k such that A360121(k) is odd, but A347389(k) is even.
 * https://oeis.org/A360124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer k ≥ 1 such that k is divisible
 *            by 8 and the odd part of k is a perfect square. Equivalently, k
 *            = 2^e * m^2 with e ≥ 3 and m odd, sorted in increasing order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the offset is 1, so n=0 is unused */

    /* The smallest exponent of 2 allowed is 3 (k must be a multiple of 8).  */
    static const idx_t MIN_EXP  = 3;
    static const idx_t MAX_EXP  = 63;          /* 2^63 fits into 64‑bit and 128‑bit seq_t */
    static const idx_t NUM_EXP = MAX_EXP - MIN_EXP + 1;

    /* Candidate structure: current value k, exponent e, odd factor m.  */
    struct Candidate {
        seq_t k;
        idx_t e;
        seq_t m;
    };

    struct Candidate cand[NUM_EXP];
    seq_t pow2[NUM_EXP];

    /* Pre‑compute powers of two from 2^MIN_EXP up to 2^MAX_EXP.  */
    seq_t p = (seq_t)1;
    idx_t i;
    for (i = 0; i < MIN_EXP; ++i) p *= (seq_t)2;
    pow2[0] = p;
    for (i = 1; i < NUM_EXP; ++i) {
        p *= (seq_t)2;
        pow2[i] = p;
    }

    /* Initialise each exponent's candidate with m = 1 (i.e. the smallest odd square).  */
    for (i = 0; i < NUM_EXP; ++i) {
        cand[i].e = MIN_EXP + i;
        cand[i].m = (seq_t)1;
        cand[i].k = pow2[i];               /* k = 2^e * 1^2 */
    }

    idx_t cnt = 0;
    for (;; ) {
        /* Find the smallest candidate.  */
        seq_t min_val = cand[0].k;
        idx_t min_idx = 0;
        for (i = 1; i < NUM_EXP; ++i) {
            if (cand[i].k < min_val) {
                min_val = cand[i].k;
                min_idx = i;
            }
        }

        ++cnt;
        if (cnt == n) return min_val;      /* return the n‑th term */

        /* Advance the chosen exponent: increase m by 2 (next odd integer). */
        cand[min_idx].m += (seq_t)2;
        seq_t m2 = cand[min_idx].m * cand[min_idx].m;
        cand[min_idx].k = pow2[min_idx] * m2;
    }
}
