/*
 * A373712  a(n) is the least m >= 0 with the same number of ternary digits as
 *          n such that for some permutation p of 0..2, applying p to the
 *          ternary digits of n yields the ternary digits of m.
 * https://oeis.org/A373712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A373712: a(n) = least m ≥ 0 with the same number of
 * ternary digits as n such that some permutation of the ternary digits of
 * n yields the ternary digits of m.  For n = 0 the result is 0.
 *
 * The algorithm:
 * 1. Convert n to base‑3 digits (most significant first, ignoring leading
 *    zeros).  For n = 0 we treat the single digit 0.
 * 2. Enumerate all 6 permutations of the digit set {0,1,2}.  For each
 *    permutation that does NOT map the most significant digit to 0
 *    (i.e., the resulting number would have the same number of digits),
 *    compute the corresponding integer value.
 * 3. Return the smallest of these values.  It is guaranteed that this
 *    value is ≤ n, so we initialise the best candidate with (seq_t)n.
 */
#include <stdbool.h>

static const uint8_t _perms[6][3] = {
    {0, 1, 2},
    {0, 2, 1},
    {1, 0, 2},
    {1, 2, 0},
    {2, 0, 1},
    {2, 1, 0}
};

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* special case: a(0)=0 */

    /* Step 1: extract ternary digits of n, most significant first */
    uint8_t digits[64];
    idx_t len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = temp % 3;
        temp /= 3;
    }
    /* digits now hold LSB first. Reverse to MSB first */
    for (idx_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t best = (seq_t)n;          /* a(n) <= n */
    uint8_t msd = digits[0];

    /* Step 2 & 3: evaluate every permissible permutation */
    for (int p = 0; p < 6; ++p) {
        uint8_t map0 = _perms[p][msd];
        if (map0 == 0) continue;      /* would lose a leading digit */

        seq_t val = 0;
        for (idx_t i = 0; i < len; ++i) {
            uint8_t mapped = _perms[p][digits[i]];
            val = val * 3 + (seq_t)mapped;
        }
        if (val < best) best = val;
    }

    return best;
}
