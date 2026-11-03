/*
 * A308584  Number of ways to write n as a*(a+1)/2 + b*(b+1)/2 + 5^c*8^d,
 *          where a,b,c,d are nonnegative integers with a <= b.
 * https://oeis.org/A308584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (a,b,c,d) with * a <= b, a,b,c,d >=
 *            0 such that * n = a*(a+1)/2 + b*(b+1)/2 + 5^c * 8^d
 * Range:     u8 n <= 886, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8651 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff x can be written as 5^c * 8^d with
 * c,d >= 0.  The implementation checks that the
 * exponent of 2 in x is a multiple of 3 and the
 * remaining factor is a power of 5. */
static inline bool is_power_5_and_8_mult3(seq_t x)
{
    if (x == 0) return false;      /* product is at least 1 */

    unsigned int exp2 = 0;
    seq_t t = x;

    /* extract factors of 2 */
    while ((t & 1) == 0) {         /* equivalent to t % 2 == 0 */
        t >>= 1;
        exp2++;
    }

    /* exponent of 2 must be multiple of 3 (for 8^d) */
    if (exp2 % 3 != 0) return false;

    unsigned int exp5 = 0;

    /* extract factors of 5 */
    while (t % 5 == 0) {
        t /= 5;
        exp5++;
    }

    /* after dividing out all powers of 2 and 5 we should
     * be left with 1.  Any other residue means x
     * cannot be written as 5^c * 8^d. */
    return t == 1;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = (seq_t)n;        /* cast index to seq_t exactly */

    /* iterate over a (triangle index) */
    for (idx_t a = 0; ; ++a) {
        seq_t Ta = ((seq_t)a * ((seq_t)a + 1)) / 2;
        if (Ta > N)
            break;                 /* no larger a can fit */

        /* iterate over b (>= a) */
        for (idx_t b = a; ; ++b) {
            seq_t Tb = ((seq_t)b * ((seq_t)b + 1)) / 2;
            seq_t sum_ab = Ta + Tb;
            if (sum_ab > N)
                break;             /* larger b will only increase sum_ab */

            seq_t r = N - sum_ab;
            if (is_power_5_and_8_mult3(r))
                count++;
        }
    }

    return count;
}
