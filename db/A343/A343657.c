/*
 * A343657  Sum of number of divisors of x^y for each x >= 1, y >= 0, x + y =
 *          n.
 * https://oeis.org/A343657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A000005(k^(n-k)) (divisor count of k^(n-k))
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 432, u64 n <= 8165,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"   /* provides void build_spf(idx_t limit, idx_t **spf_out); */

seq_t a(idx_t n) {
    /* Handle the offset: a(1) is the first term. */
    if (n == 0) return 0; /* undefined, but safe */

    /* Cache the smallest prime factor sieve to avoid rebuilding it. */
    static idx_t max_spf_n = 0;
    static idx_t *spf = NULL;
    if (spf == NULL || n > max_spf_n) {
        free(spf);
        build_spf(n, &spf);          /* allocates array with indices 0..n */
        max_spf_n = n;
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t tmp = k;
        seq_t term = 1;
        while (tmp > 1) {
            uint64_t p = spf[tmp];
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            seq_t factor = (seq_t)exp * (seq_t)(n - k) + 1;
            term *= factor;
        }
        sum += term;
    }

    return sum;
}
