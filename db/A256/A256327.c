/*
 * A256327  Number of partitions of 5n into exactly 4 parts.
 * https://oeis.org/A256327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of 5n into exactly 4 parts.
 *
 * The algorithm uses the identity
 *
 *   p_exact4(m) = p_atmost4(m) – p_atmost3(m),
 *
 * where p_atmostk(m) is the number of partitions of m into at most k parts.
 * The latter can be computed by a simple dynamic programming that
 * counts partitions into parts of size at most k (Ferrers' conjugate).
 * The tables are built only up to 5*n for each call, which is fast
 * enough for n ≤ 10000.
 *
 * The implementation is split between C and C++ to handle arbitrary‑precision
 * `seq_t` types. In C, `calloc` and `free` are used; in C++ a `std::vector`
 * is employed so that constructors are called automatically.  All code
 * follows the type conventions described in the problem statement.
 */

#include <stdlib.h>   /* for calloc, free, abort */
#ifdef __cplusplus
#include <vector>
#endif

/* External linkage function required by the build system */
seq_t a(idx_t n)
{
    /* sum whose partitions we need */
    idx_t m = 5 * n;

    /* Special case m = 0: both tables equal 1, difference is 0 */
    if (m == 0) {
        return (seq_t)0;
    }

#ifdef __cplusplus
    /* C++ implementation using std::vector (automatic construction) */
    std::vector<seq_t> dp3(m + 1, seq_t(0));
    std::vector<seq_t> dp4(m + 1, seq_t(0));

    dp3[0] = seq_t(1);
    for (idx_t part = 1; part <= 3; ++part) {
        for (idx_t sum = part; sum <= m; ++sum) {
            dp3[sum] += dp3[sum - part];
        }
    }

    dp4[0] = seq_t(1);
    for (idx_t part = 1; part <= 4; ++part) {
        for (idx_t sum = part; sum <= m; ++sum) {
            dp4[sum] += dp4[sum - part];
        }
    }

    return dp4[m] - dp3[m];
#else
    /* C implementation using calloc (trivial zeroing) */
    seq_t *dp3 = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    seq_t *dp4 = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    if (!dp3 || !dp4) { abort(); }

    dp3[0] = (seq_t)1;
    for (idx_t part = 1; part <= 3; ++part) {
        for (idx_t sum = part; sum <= m; ++sum) {
            dp3[sum] += dp3[sum - part];
        }
    }

    dp4[0] = (seq_t)1;
    for (idx_t part = 1; part <= 4; ++part) {
        for (idx_t sum = part; sum <= m; ++sum) {
            dp4[sum] += dp4[sum - part];
        }
    }

    seq_t result = dp4[m] - dp3[m];
    free(dp3);
    free(dp4);
    return result;
#endif
}
