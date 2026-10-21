/*
 * A258692  Integers n such that n*(n + 2)*(n + 4) + 1 is a perfect square.
 * https://oeis.org/A258692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   integers n such that n*(n+2)*(n+4)+1 is a perfect square (a(n)
 *            is the nth such integer, OEIS A258692)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t sols[32];
    static size_t solcnt = 0;

    if (!init) {
        init = true;
        const int MAX = 2000;                      /* Search range wide enough to capture all known solutions */
        for (long long i = -(long long)MAX; i <= MAX; ++i) {
            seq_t sn = (seq_t)i;
            seq_t prod = sn * (sn + (seq_t)2);
            prod = prod * (sn + (seq_t)4);
            seq_t m = prod + (seq_t)1;
            if (m < (seq_t)0) continue;
            seq_t sq = floor_sqrt(m);
            if (sq * sq == m) {
                if (solcnt < sizeof(sols) / sizeof(*sols))
                    sols[solcnt++] = sn;
            }
        }

        /* Sort the found solutions in ascending order */
        for (size_t i = 1; i < solcnt; ++i) {
            seq_t key = sols[i];
            size_t j = i;
            while (j > 0 && sols[j - 1] > key) {
                sols[j] = sols[j - 1];
                --j;
            }
            sols[j] = key;
        }
    }

    if (n == 0) return 0;          /* Undefined for offset zero */
    if (n <= solcnt) return sols[n - 1];
    return 0;                      /* No further terms (sequence is finite) */
}
