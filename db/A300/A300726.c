/*
 * A300726  Difference between A053644 (the largest power of 2 less than or
 *          equal to n) and its Möbius transform.
 * https://oeis.org/A300726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A053644(n) - A300724(n)
 * Range:     u8 n <= 511, u16 n <= 8685, u32 n <= 8685, u64 n <= 8685,
 *            u128 n <= 8685, big any n
 * Helpers:   factorize, floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = -∑_{d|n, d>1} µ(d)·A053644(n/d)   */
/* Uses helper functions: factorize, pow2, floor_log2_uint64 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorize.h"
#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

static void rec_divisors(idx_t n,
                         const uint64_t *primes,
                         const int *exps,
                         size_t total,
                         size_t pos,
                         uint64_t curr,
                         int odd_cnt,
                         int zero_flag,
                         long long *sum)
{
    if (pos == total) {
        if (curr > 1 && zero_flag == 0) {
            int mu = (odd_cnt & 1) ? -1 : 1;
            idx_t nd = n / curr;
            seq_t fnd = pow2(floor_log2_uint64(nd));
            long long val = (long long)fnd;
            if (mu == 1) {
                *sum += val;
            } else {
                *sum -= val;
            }
        }
        return;
    }

    uint64_t p = primes[pos];
    uint64_t mult = 1;
    for (int e = 0; e <= exps[pos]; ++e) {
        int nz = zero_flag;
        int oc = odd_cnt;
        if (e >= 2) nz = 1;
        if (e == 1) ++oc;
        rec_divisors(n, primes, exps, total, pos + 1,
                     curr * mult, oc, nz, sum);
        mult *= p;
    }
}

seq_t a(idx_t n) {
    uint64_t primes[64];
    int exps[64];
    size_t total = factorize(n, primes, exps);

    long long sum = 0;
    rec_divisors(n, primes, exps, total, 0, 1, 0, 0, &sum);

    return (seq_t)(-sum);
}
