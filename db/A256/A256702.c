/*
 * A256702  Nonpositive part of the minimal alternating binary representation
 *          of n (defined at A256696).
 * https://oeis.org/A256702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 896 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A256702(n): the sum of negative terms in the minimal alternating
 * binary representation of n.
 *
 * Recurrence:
 *   If n is a power of two: a(n) = 0, positive part = n.
 *   Otherwise let p = 2^{⌈log₂(n)⌉} (smallest power of two strictly greater than n)
 *   and r = p - n.
 *   Let (pos, neg) be the positive and negative parts of the representation of r.
 *   Then for n:
 *      positive part = p + neg
 *      negative part = pos
 *   The function returns the negative part.
 */
#include <stdint.h>

static void compute_mabr(idx_t m, seq_t *pos, seq_t *neg)
{
    /* base case: m is a power of two */
    if ((m & (m - 1)) == 0) {
        *pos = (seq_t)m;
        *neg = 0;
        return;
    }
    /* number of bits needed to represent m */
    unsigned bl = 64 - __builtin_clzll(m);   /* m ≥ 1 */
    seq_t p = ((seq_t)1) << bl;              /* next power of two > m */
    idx_t r = (idx_t)(p - (seq_t)m);        /* remainder < m */

    seq_t pos_r, neg_r;
    compute_mabr(r, &pos_r, &neg_r);

    *pos = p + neg_r;
    *neg = pos_r;
}

seq_t a(idx_t n)
{
    seq_t pos, neg;
    compute_mabr(n, &pos, &neg);
    return neg;
}
