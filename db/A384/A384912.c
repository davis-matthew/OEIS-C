/*
 * A384912  The number of unordered factorizations of n into exponentially
 *          squarefree prime powers (A384419).
 * https://oeis.org/A384912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A384912: number of unordered factorizations of n into 
   exponentially squarefree prime powers.
   For a prime power p^e, a(p^e) = number of partitions of e
   into squarefree summands (see A073576(e)).  
   a(n) is multiplicative. 
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* precompute number of partitions of e (0 ≤ e ≤ 63) with squarefree parts */
    static seq_t part[64];
    static int init = 0;

    if (!init) {
        const int MAXE = 63;            /* max exponent for 64‑bit n (2^63 < 2^64) */
        int sf[64], cnt = 0;           /* squarefree numbers up to MAXE */

        /* collect squarefree numbers */
        for (int i = 1; i <= MAXE; ++i) {
            int sqfree = 1;
            for (int j = 2; j * j <= i; ++j) {
                if (i % (j * j) == 0) { sqfree = 0; break; }
            }
            if (sqfree) sf[cnt++] = i;
        }

        /* dp[0] = 1, others 0 */
        part[0] = 1;
        for (int i = 1; i <= MAXE; ++i) part[i] = 0;

        /* coin‑changed DP: unordered partitions with allowed parts */
        for (int p = 0; p < cnt; ++p) {
            int val = sf[p];
            for (int i = val; i <= MAXE; ++i)
                part[i] += part[i - val];
        }
        init = 1;
    }

    /* factor n and multiply partition counts */
    uint64_t m = (uint64_t)n;
    seq_t res = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            res *= part[e];
        }
    }
    if (m > 1)    /* remaining prime factor with exponent 1 */
        res *= part[1];

    return res;
}
