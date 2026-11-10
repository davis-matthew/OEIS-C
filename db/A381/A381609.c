/*
 * A381609  a(n) is the number of occurrences of n in A381607.
 * https://oeis.org/A381609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of (t_0,t_1,...) with t_k in {0,1,2}, finitely
 *            many nonzero, such that n = Sum_{k>=0} t_k*A000045(2k+2); this
 *            is the number of occurrences of n in A381607 (counted by pruned
 *            recursion)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9261 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A381609_u128;

static unsigned long long A381609_count(const A381609_u128 *E, const A381609_u128 *cap,
                                        A381609_u128 rest, int k)
{
    if (k < 0) return rest == 0 ? 1ULL : 0ULL;
    unsigned long long total = 0;
    for (int t = 0; t <= 2; ++t) {
        A381609_u128 use = (A381609_u128)t * E[k];
        if (use > rest) break;
        A381609_u128 r = rest - use;
        if (r > cap[k]) continue;
        total += A381609_count(E, cap, r, k - 1);
    }
    return total;
}

seq_t a(idx_t n)
{
    A381609_u128 E[128];
    A381609_u128 cap[128];
    int cnt = 0;
    A381609_u128 N = (A381609_u128)n;

    E[0] = 1;
    E[1] = 3;
    cnt = 2;
    while (E[cnt - 1] <= N && cnt < 120) {
        E[cnt] = 3 * E[cnt - 1] - E[cnt - 2];
        ++cnt;
    }
    /* cap[k] = max sum achievable using indices < k = 2*Sum_{j<k} E[j] */
    cap[0] = 0;
    for (int k = 1; k < cnt; ++k)
        cap[k] = cap[k - 1] + 2 * E[k - 1];

    unsigned long long c = A381609_count(E, cap, N, cnt - 1);
    return (seq_t)c;
}
