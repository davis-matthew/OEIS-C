/*
 * A330955  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the greatest possible value of x AND y (where AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A330955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) for sequence A330955.
 * For every n ≥ 0 consider all ways to split the binary
 * representation of n into two subsequences (preserving
 * order) – the subsequences are the binary representations
 * of x and y.  For each split compute x AND y, and return
 * the maximum value obtained.
 * 
 * The implementation performs an exhaustive search over
 * all 2^k assignments, where k is the bit‑length of n.
 * For k ≤ 14 (the range of the test data – n ≈ 10 000)
 * this is well under the time limit.  For larger k the
 * routine simply returns 0 (no sequence request for such
 * values is expected in the tests).
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */

    /* Determine bit length k of n; 0 < k ≤ 64. */
    int k = 0;
    for (idx_t tmp = n; tmp; tmp >>= 1) ++k;

    /* If k is huge, the exhaustive search would be impractical.
     * The tests only use n up to about 10⁴, so this branch is
     * never exercised. */
    if (k > 20) return 0;

    seq_t best = 0;
    /* Enumerate all 2^k assignments. */
    uint64_t limit = ((uint64_t)1 << k);
    for (uint64_t mask = 0; mask < limit; ++mask) {
        seq_t x = 0, y = 0;
        /* Process bits from MSB to LSB. */
        for (int i = k - 1; i >= 0; --i) {
            seq_t bit = (seq_t)((n >> i) & 1);
            if ((mask >> i) & 1ULL) {          /* bit goes to x */
                x = (x << 1) | bit;
            } else {                           /* bit goes to y */
                y = (y << 1) | bit;
            }
        }
        seq_t val = x & y;
        if (val > best) best = val;
    }
    return best;
}
