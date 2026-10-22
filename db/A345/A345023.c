/*
 * A345023  a(n) is the surface area of the symmetric tower described in
 *          A221529 which is a polycube whose successive terraces are the
 *          symmetric representation of sigma A000203(i) (from i = 1 to n)
 *          starting from the top and the levels of these terraces are the
 *          partition numbers A000041(h-1) (from h = 1 to n)...
 * https://oeis.org/A345023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A000070(n-1) + 2*A024916(n)
 * Range:     u8 n <= 7, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    where A000070(k) = sum_{i=0}^k p(i)   (partition number p(i))*/
/*          A024916(n) = sum_{i=1}^n sigma(i)   (sum of divisors sigma) */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1, but guard against n==0 */

    idx_t N = n;               /* number of partitions to compute */
    /*--- Partition numbers up to N-1 and their cumulative sums ---*/
#ifdef __cplusplus
    std::vector<seq_t> part(N, 0);
    std::vector<seq_t> cum(N, 0);
#else
    seq_t *part = calloc(N, sizeof(seq_t));
    seq_t *cum  = calloc(N, sizeof(seq_t));
#endif

    part[0] = 1;                /* p(0) = 1 */

    /* classic dp: part[j] += part[j-i]   for i upto N-1 */
    for (idx_t i = 1; i < N; ++i) {
        for (idx_t j = i; j < N; ++j) {
            part[j] += part[j - i];
        }
    }

    cum[0] = part[0];
    for (idx_t i = 1; i < N; ++i) {
        cum[i] = cum[i - 1] + part[i];
    }
    seq_t sum_part = cum[N - 1];   /* A000070(N-1) */

    /*--- Sigma(i) and their prefix sums up to N ---*/
#ifdef __cplusplus
    std::vector<seq_t> sigma(N + 1, 0);
#else
    seq_t *sigma = calloc(N + 1, sizeof(seq_t));
#endif

    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; j += i) {
            sigma[j] += (seq_t)i;  /* i divides j */
        }
    }

    seq_t sum_sigma = 0;
    for (idx_t i = 1; i <= N; ++i) {
        sum_sigma += sigma[i];
    }

    seq_t result = (seq_t)4 * sum_part + (seq_t)2 * sum_sigma;

#ifndef __cplusplus
    free(part);
    free(cum);
    free(sigma);
#endif

    return result;
}
