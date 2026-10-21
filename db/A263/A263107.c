/*
 * A263107  Number of ordered pairs (k, m) with k > 0 and m > 0 such that n =
 *          pi(k^2/2) + pi(3*m^2/2), where pi(x) denotes the number of primes
 *          not exceeding x.
 * https://oeis.org/A263107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{(k,m) with k>0, m>0 | pi(k^2/2)+pi(3*m^2/2)=n}
 * where pi(x) = number of primes ≤ x.
 *
 * The function counts all ordered pairs (k,m) that satisfy the
 * equation for the given n.  It uses the utility function
 * compute_pi(idx_t) from ../utils/compute_pi.h to obtain prime counts.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n<1, but return 0 */

    /* Frequency table for values of pi(3*m^2/2), indexed by the pi‑value. */
    uint64_t *freq = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    if (!freq) return 0;                  /* allocation failure → 0 */

    /* Initialize frequency array. */
    for (idx_t i = 0; i <= n; ++i) freq[i] = 0;

    /* Count how many m give each possible value of pi(3*m^2/2) ≤ n. */
    for (idx_t m = 1; ; ++m) {
        __uint128_t v128 = (__uint128_t)3 * m * m / 2;
        if (v128 > UINT64_MAX) break;    /* value no longer fits in idx_t */
        idx_t v = (idx_t)v128;
        seq_t pi_v = compute_pi(v);
        if (pi_v > n) break;
        idx_t idx = (idx_t)pi_v;
        freq[idx]++;
    }

    seq_t ans = 0;

    /* For each k, add the number of m that produce the complementary
       pi(3*m^2/2) value. */
    for (idx_t k = 1; ; ++k) {
        __uint128_t v128 = (__uint128_t)k * k / 2;
        if (v128 > UINT64_MAX) break;
        idx_t v = (idx_t)v128;
        seq_t pi_k = compute_pi(v);
        if (pi_k > n) break;
        idx_t pi_k_i = (idx_t)pi_k;
        idx_t target = n - pi_k_i;        /* required pi(3*m^2/2) value */
        if (target <= n) {
            ans += (seq_t)freq[target];
        }
    }

    free(freq);
    return ans;
}
