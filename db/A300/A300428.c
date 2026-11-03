/*
 * A300428  a(n) is the least positive k such that the binary representation
 *          of n appears as a substring in the binary representation of 1/k
 *          (ignoring the radix point and adding trailing zeros if necessary
 *          in case of a terminating expansion).
 * https://oeis.org/A300428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > 0 such that the binary representation of n
 *            appears as a substring in the infinite binary expansion of 1/k
 *            (ignore the radix point, add trailing zeros if 1/k terminates).
 *            All values a(n) are odd
 * Range:     u8 n <= 73, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 89 terms through n = 7216 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff the binary pattern of n (of length L bits, mask=2^L-1)
   occurs in the binary expansion of 1/k.  */
static bool pattern_in_fraction(seq_t n, seq_t k, int L, seq_t mask)
{
    if (k == 0) return false;          /* never happens, just safe */

    seq_t Q = (seq_t)1 / k;                  /* floor(2^0 / k)  */
    seq_t R = (seq_t)1 % k;                  /* remainder 1 % k */
    /* Upper bound for the number of steps: twice the denominator plus
       a small constant.  This is enough because the period of the
       expansion of 1/k divides k-1 (for odd k).  */
    idx_t limit = (idx_t)(2 * ( (seq_t)k + (seq_t)L ) + 10);

    for (idx_t i = 0; i < limit; ++i) {
        if (Q == n) return true;
        seq_t t = R << 1;                   /* 2*R   */
        seq_t carry = t / k;                /* 0 or 1   */
        R = t % k;
        Q = ((Q << 1) + carry) & mask;     /* keep only low L bits   */
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, n>=1 in use */

    seq_t nn = n;

    /* Powers of two get output 1.  */
    if ((nn & (nn - 1)) == 0) return 1;

    /* Numbers of the form 2^k-1 (k>1) get output 2^k+1 = n+2.  */
    if (nn > 1) {
        seq_t t = nn + 1;
        if ((t & (t - 1)) == 0) return nn + 2;
    }

    /* Length of binary representation of n. */
    int L = 0;
    seq_t tmp = nn;
    while (tmp) { ++L; tmp >>= 1; }
    if (L == 0) L = 1;                    /* for n==0, not used */

    seq_t mask = ((seq_t)1 << L) - 1;      /* 2^L - 1 */

    /* Search for the smallest odd k that satisfies the condition.  */
    for (seq_t k = 1; ; ++k) {
        /* All terms are odd, skip even denominators (except 1).  */
        if (k > 1 && (k & 1) == 0) continue;
        if (pattern_in_fraction(nn, k, L, mask)) return k;
    }
}
