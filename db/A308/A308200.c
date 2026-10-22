/*
 * A308200  The tribonacci representation of a(n) is obtained by appending
 *          0,0,0 to the tribonacci representation of n (cf. A278038).
 * https://oeis.org/A308200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ T_{i+3} for all i where the Tribonacci representation
 *            of n has a 1 at position i (cf. A278038)
 * Range:     u8 n <= 41, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const size_t MAX_TRIB = 64;    /* enough for n up to at least 10⁵ */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t  val   = (seq_t)n;          /* remaining part of n to decompose   */
    seq_t  trib[MAX_TRIB];
    size_t len   = 0;                 /* number of tribonacci values stored */

    /* Initialise the first three tribonacci numbers */
    trib[0] = (seq_t)0;
    trib[1] = (seq_t)0;
    trib[2] = (seq_t)1;
    len = 3;

    /* Generate tribonacci numbers until we surpass val */
    while (trib[len - 1] <= val) {
        trib[len] = trib[len - 1] + trib[len - 2] + trib[len - 3];
        if (++len >= MAX_TRIB)
            break;   /* safety guard */
    }

    /* We need digits up to index (lastIndex + 3); generate two more if required */
    size_t baseLen   = len;            /* len after the first loop        */
    size_t targetLen = baseLen + 2;    /* need indices up to baseLen+1    */
    while (len < targetLen) {
        trib[len] = trib[len - 1] + trib[len - 2] + trib[len - 3];
        ++len;
    }

    seq_t result = (seq_t)0;
    size_t lastIndex = baseLen - 2;    /* largest i with T_i <= val */

    /* Greedy conversion of val to tribonacci representation,
       simultaneously accumulating the shifted tribonacci sum. */
    for (size_t i = lastIndex;; --i) {
        if (val >= trib[i]) {
            val -= trib[i];
            result += trib[i + 3];
        }
        if (i == 3)
            break;
    }

    return result;
}
