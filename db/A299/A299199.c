/*
 * A299199  In factorial base, any rational number has a terminating
 *          expansion; hence we can devise a self-inverse permutation of the
 *          rational numbers, say f, such that for any rational number q, the
 *          representations of q and of f(q) in factorial base are mirrored
 *          around the radix point and q and f(q) have th...
 * https://oeis.org/A299199
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = f(1/n) where f mirrors the factorial‑base digits of 1/n
 *            around the radix point. For n ≥ 2 this computes the terminating
 *            factorial expansion of 1/n, reverses all digits except the
 *            leading 0 (which occurs for all n > 1), and interprets the
 *            reversed digits as a factorial‑base integer. The res...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; values below are unused */
    if (n < 2) return (seq_t)0;

    seq_t n_seq = (seq_t)n;          /* use seq_t for all arithmetic   */
    /* Temporary array for the factorial digits of 1/n.
       The length m is at most n (Smarandache function ≤ n for n ≥ 2).   */
    uint32_t *d = (uint32_t *)malloc((n + 2) * sizeof(uint32_t));
    if (!d) return (seq_t)0;         /* unlikely, but safe            */

    uint32_t m = 0;                  /* number of digits actually found */
    seq_t t = 1;                     /* running numerator for the loop   */

    /* Compute the factorial digits d[0]…d[m-1] of 1/n.
       d[0] will always be 0 for n ≥ 2.                                 */
    while (1) {
        t *= (m + 1);                /* multiply by i (i = m+1)          */
        d[m] = (uint32_t)(t / n_seq);/* quotient is the digit d_i        */
        t %= n_seq;                  /* remainder for next step           */
        m++;
        if (t == 0) break;           /* terminating expansion reached     */
    }

    /* The reversed digit sequence (excluding the initial d[0]) has
       length L = m - 1.  rev[j] = d[m-1-j] for 0 ≤ j < L.           */
    uint32_t L = (m > 0) ? (m - 1) : 0;
    seq_t val = 0;
    for (uint32_t j = 0; j < L; ++j) {
        val = val * ((seq_t)(L - j + 1)) + (seq_t)d[(int)(m - 1 - j)];
    }

    free(d);
    return val;
}
