/*
 * A363211  Number of partitions of n such that 3*(least part) <= greatest
 *          part.
 * https://oeis.org/A363211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that 3 * (least part) <=
 *            (greatest part)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses partition dynamic programming.
   Firstly p(n) (all partitions) is computed.
   Then for each possible least part m we count the
   partitions that violate the condition (i.e. all parts
   lie in the interval [m, 3m-1]) and subtract them
   from p(n).  The partitions that violate the condition
   are precisely those with minimum part exactly m and
   maximum part at most 3m-1.  These are obtained by
   DP over the range [m,3m-1] and subtracting the case
   where the part m is not used.  */
#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t N = n;

    /* Partition numbers p(n) */
    seq_t *dp_part = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp_part) return 0;
    for (idx_t i = 0; i <= N; ++i) dp_part[i] = 0;
    dp_part[0] = 1;
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t t = k; t <= N; ++t) {
            dp_part[t] += dp_part[t - k];
        }
    }
    seq_t p_n = dp_part[N];
    free(dp_part);

    /* Count violating partitions */
    seq_t violating = 0;
    seq_t *dp_range = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp_range) return p_n; /* safe fallback */

    for (idx_t m = 1; m <= N; ++m) {
        /* Upper bound of parts in the interval */
        idx_t L = 3 * m - 1;
        if (L > N) L = N;

        /* Initialize dp_range to zero */
        for (idx_t i = 0; i <= N; ++i) dp_range[i] = 0;
        dp_range[0] = 1;

        /* Count partitions with parts in [m+1, L] */
        for (idx_t k = m + 1; k <= L; ++k) {
            for (idx_t t = k; t <= N; ++t) {
                dp_range[t] += dp_range[t - k];
            }
        }
        seq_t P_range_mplus1 = dp_range[N];

        /* Add the part size m (unlimited repetitions) */
        for (idx_t t = m; t <= N; ++t) {
            dp_range[t] += dp_range[t - m];
        }
        seq_t P_range_m = dp_range[N];

        seq_t B = P_range_m - P_range_mplus1; /* partitions with min=m and max ≤ 3m-1 */
        violating += B;
    }
    free(dp_range);

    /* Valid partitions = all partitions – violating partitions */
    return p_n - violating;
}
