/*
 * A338554  Number of non-constant integer partitions of n whose parts have a
 *          common divisor > 1.
 * https://oeis.org/A338554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A018783(n) - A000005(n) + 1
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A018783(n) = -∑_{d>1, d|n} μ(d)·p(n/d) and p(m) is the partition number. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* cache of partition numbers up to the largest n requested so far */
static seq_t *p_cache = NULL;
static idx_t p_cache_size = 0;

/* generate partition numbers p[0..N] using pentagonal number theorem */
static void partition_cache(idx_t N)
{
    seq_t *newp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!newp) return;
    newp[0] = 1;
    for (idx_t m = 1; m <= N; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            if (k & 1)      sum += newp[m - g1];
            else            sum -= newp[m - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            if (k & 1)      sum += newp[m - g2];
            else            sum -= newp[m - g2];
        }
        newp[m] = sum;
    }
    free(p_cache);
    p_cache = newp;
    p_cache_size = N + 1;
}

/* Möbius function μ(n) for n ≥ 1 */
static int mobius(uint64_t n)
{
    if (n == 1) return 1;
    int mu = 1;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return 0;
            mu = -mu;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) mu = -mu;
    return mu;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* make sure partition numbers up to n are available */
    if (n + 1 > p_cache_size) partition_cache(n);

    /* divisor count d(n) */
    uint64_t divisor_count = 0;
    for (idx_t d = 1; (uint64_t)d * d <= n; ++d) {
        if (n % d == 0) {
            if ((uint64_t)d * d == n) divisor_count += 1;
            else                       divisor_count += 2;
        }
    }

    /* A018783(n) = -∑_{d>1, d|n} μ(d)·p(n/d) */
    seq_t sum_neg = 0;   /* μ(d) = -1 contributes +p(n/d) */
    seq_t sum_pos = 0;   /* μ(d) = +1 contributes -p(n/d) */
    for (idx_t d = 2; d <= n; ++d) {
        if (n % d == 0) {
            int mu = mobius(d);
            idx_t m = n / d;
            if (mu == 1)      sum_pos += p_cache[m];
            else if (mu == -1)sum_neg += p_cache[m];
            /* μ(d)==0 contributes nothing */
        }
    }
    seq_t A018783 = sum_neg - sum_pos;  /* equals -∑ μ(d)·p(n/d) */

    /* a(n) = A018783(n) - d(n) + 1 */
    seq_t result = A018783 - (seq_t)divisor_count + 1;
    return result;
}
