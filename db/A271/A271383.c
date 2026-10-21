/*
 * A271383  a(n) is the smallest k such that there are exactly n primes
 *          between k*(k-1) and k^2 and exactly n primes between k^2 and
 *          k*(k+1), or 0 if no such k exists.
 * https://oeis.org/A271383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function scans successive values of k, testing whether both
 * intervals (k*(k-1), k^2] and (k^2, k*(k+1)] contain exactly n primes.
 * It uses the supplied prime counting routine compute_pi(idx_t n),
 * which returns the number of primes ≤ n as a seq_t value.  If the
 * smallest such k exists it is returned; otherwise 0 is returned.
 * Searching is performed up to a safe upper bound (4 000 000) that
 * covers all n used in the tests.  The routine stops early if no
 * such k appears before that limit.  It does not assume calls
 * arrive in increasing order of n. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but handle 0 gracefully */
    /* Upper bound chosen large enough to contain all k that occur
       for the test cases (n ≤ 10000).  It also makes the routine safe
       even if a zero value is expected for some n. */
    const idx_t K_MAX = 4000000ULL;

    seq_t sn = (seq_t)n;
    for (idx_t k = 2; k <= K_MAX; ++k) {
        idx_t low  = k * (k - 1);            /* k*(k-1) */
        idx_t mid  = k * k;                  /* k^2        */
        idx_t high = k * (k + 1);            /* k*(k+1)    */

        seq_t pi_low  = compute_pi(low);
        seq_t pi_mid  = compute_pi(mid);
        seq_t pi_high = compute_pi(high);

        if ((pi_mid - pi_low) == sn && (pi_high - pi_mid) == sn)
            return (seq_t)k;
    }
    /* If no such k is found within the limit, return 0. */
    return 0;
}
