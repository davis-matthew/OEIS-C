/*
 * A386637  Numbers k such that sigma(k) = psi(k) + tau(k) + omega(k).
 * https://oeis.org/A386637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 7 * p^2 where p is the n-th prime excluding 7
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute an upper bound on the (n+1)-th prime (to skip the prime 7) */
    idx_t limit = _prime_limit_for_idx(n + 1 + 1);   /* +1 for 7, +1 buffer */
    size_t cnt = 0;
    uint64_t *pr = NULL;

    /* build the list of primes up to the limit */
    build_primes_upto(limit, &pr, &cnt);

    /* iterate to find the n-th prime excluding 7 */
    idx_t target = n;
    idx_t found = 0;
    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 7ULL)
            continue;                   /* skip the prime 7 */
        ++found;
        if (found == target) {
            seq_t p = (seq_t)pr[i];
            seq_t p2 = seq_mul(p, p);    /* p^2 */
            result = seq_mul(p2, (seq_t)7);  /* 7 * p^2 */
            break;
        }
    }

    free(pr);
    return result;
}
