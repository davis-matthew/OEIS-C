/*
 * A381497  a(n) = sum of numbers k < n such that 1 < gcd(k,n) and rad(k) !=
 *          rad(n), where rad = A007947.
 * https://oeis.org/A381497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over 1 ≤ k < n with 1<gcd(k,n) and rad(k)!=rad(n) of
 *            k
 * Range:     u8 n <= 29, u16 n <= 419, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   dfs_same_rad, factorize, phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorize.h"
#include "../utils/rad.h"
#include "../utils/phi.h"
#include "../utils/dfs_same_rad.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t total_sum = (n_seq * (n_seq - 1)) / 2;   /* sum_{k=1}^{n-1} k */
    seq_t phi_n   = phi(n);
    seq_t sum_coprime = n_seq * phi_n / 2;         /* sum of k coprime to n */
    seq_t sum_gcd = total_sum - sum_coprime;       /* sum of k with gcd>1 */

    seq_t rad_seq = rad(n);
    idx_t rad_val = (idx_t)rad_seq;
    idx_t limit = (n - 1) / rad_val;               /* maximal m with rad*m < n */

    seq_t sum_same_rad = 0;
    if (limit > 0) {
        uint64_t primes[20];
        int exps[20];
        int cnt = factorize(n, primes, exps);
        dfs_same_rad(0, 1, limit, primes, cnt, rad_seq, &sum_same_rad);
    }

    return sum_gcd - sum_same_rad;
}
