/*
 * A246720  Number A(n,k) of partitions of n into parts of the k-th list of
 *          distinct parts in the order given by A246688; square array A(n,k),
 *          n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A246720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = number of partitions of n into parts from the k-th list
 *            S_k of distinct parts (order of A246688: by sum, then
 *            lexicographic); a(m) read by antidiagonals: m = d(d+1)/2 + i, n
 *            = i, k = d - i. Computed by generating S_k and coin-change DP
 * Range:     u8 n <= 1981, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9468 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned long long A246720_target, A246720_counter;
static bool A246720_found;
static unsigned long long A246720_parts[256];
static unsigned A246720_len;
static unsigned long long A246720_cur[256];

static void A246720_gen(unsigned long long rem, unsigned long long minp, unsigned depth)
{
    if (rem == 0) {
        if (A246720_counter == A246720_target) {
            A246720_found = true;
            A246720_len = depth;
            for (unsigned i = 0; i < depth; ++i) A246720_parts[i] = A246720_cur[i];
        }
        A246720_counter++;
        return;
    }
    for (unsigned long long p = minp; p <= rem; ++p) {
        A246720_cur[depth] = p;
        A246720_gen(rem - p, p + 1, depth + 1);
        if (A246720_found) return;
    }
}

seq_t a(idx_t m)
{
    unsigned long long mm = (unsigned long long)m;
    unsigned long long d = 0;
    {
        double r = (double)(8.0 * (double)mm + 1.0);
        long double s = 0;
        /* integer sqrt via double then adjust */
        unsigned long long q = (unsigned long long)(( __builtin_sqrt(r) - 1.0) / 2.0);
        (void)s;
        d = q;
        while ((d + 1) * (d + 2) / 2 <= mm) d++;
        while (d * (d + 1) / 2 > mm) d--;
    }
    unsigned long long i = mm - d * (d + 1) / 2;
    unsigned long long n = i, k = d - i;

    A246720_target = k;
    A246720_counter = 0;
    A246720_found = false;
    A246720_len = 0;
    for (unsigned long long s = 0; !A246720_found; ++s)
        A246720_gen(s, 1, 0);

    seq_t *dp = mem_new(seq_t, (size_t)n + 1);
    dp[0] = 1;
    for (unsigned j = 0; j < A246720_len; ++j) {
        unsigned long long p = A246720_parts[j];
        if (p > n) continue;
        for (unsigned long long x = p; x <= n; ++x)
            dp[x] += dp[x - p];
    }
    seq_t res = dp[n];
    mem_free(dp);
    return res;
}
