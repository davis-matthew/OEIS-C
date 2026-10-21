/*
 * A241971  Numbers congruent to some square mod 10^k for all values of k.
 * https://oeis.org/A241971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the positive integers n such that * 1. the exponent of
 *            2 in n is even (including 0); * 2. the exponent of 5 in n is
 *            even (including 0); * 3. after removing all factors of 2, the
 *            remaining odd part is ≡ 1 (mod 8); * 4. after removing all
 *            factors of 5, the remaining part is ≡ 1 or 4 (mod 5...
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool good(seq_t val)
{
    /* test exponent of 2 */
    seq_t tmp = val;
    idx_t e2 = 0;
    while ((tmp & (seq_t)1) == 0) {
        tmp >>= 1;
        e2++;
    }
    if ((e2 & 1) != 0)           /* odd exponent of 2 */
        return false;

    /* odd part after removing 2's must be 1 (mod 8) */
    seq_t odd2 = tmp;
    if ((odd2 % (seq_t)8) != 1)
        return false;

    /* test exponent of 5 */
    tmp = val;
    idx_t e5 = 0;
    while (tmp % (seq_t)5 == 0) {
        tmp /= 5;
        e5++;
    }
    if ((e5 & 1) != 0)           /* odd exponent of 5 */
        return false;

    /* odd part after removing 5's must be ≡ 1 or 4 (mod 5) */
    seq_t odd5 = tmp;
    seq_t r5 = odd5 % (seq_t)5;
    if (r5 != 1 && r5 != 4)
        return false;

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but guard anyway */

    seq_t candidate = (seq_t)1;
    idx_t count = 0;

    while (count < n) {
        if (good(candidate))
            count++;
        candidate++;
    }

    return candidate - 1;
}
