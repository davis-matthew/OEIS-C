/*
 * A320227  Assuming the truth of the Collatz conjecture, let T(n,i), i = 1..k
 *          be the initial k elements of the Collatz trajectory of n, up to
 *          when the first 1 appears, but excluding the 1. a(n) is the number
 *          of ordered pairs T(n,i) < T(n,j) such that gcd(T(n,i), T(n,j)) =
 *          1.
 * https://oeis.org/A320227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered coprime pairs among elements of the
 *            Collatz trajectory of n up to (but excluding) the first 1
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Collatz trajectory excluding the final 1 */
    /* Maximum length for n <= 10^4 is well below 4096, so a fixed array suffices. */
    static const int MAX_TRAJ = 4096;
    seq_t traj[MAX_TRAJ];
    int len = 0;

    seq_t cur = (seq_t)n;
    while (cur != (seq_t)1) {
        traj[len++] = cur;
        if (cur & (seq_t)1)            /* odd */
            cur = (seq_t)(3 * cur + 1);
        else                           /* even */
            cur = cur >> 1;
        /* In practice len will never reach MAX_TRAJ for n up to 10^4 */
    }

    seq_t count = 0;
    for (int i = 0; i < len - 1; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (gcd(traj[i], traj[j]) == (seq_t)1)
                ++count;
        }
    }
    return count;
}
