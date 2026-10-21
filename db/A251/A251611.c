/*
 * A251611  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          summing to a prime.
 * https://oeis.org/A251611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+1)×2 arrays of integers 0..6 * such
 *            that each 2×2 subblock has a prime sum. Each row is a pair (x,y)
 *            * with 0≤x,y≤6. A transition from row (x,y) to row (x′,y′) is
 *            allowed * iff (x+y+x′+y′) is prime. The total number of arrays
 *            of length n+1 * equals the number of walks...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term; for n < 1 the value is undefined */
    if (n < 1)
        return 0;

    /* ------------------------------------------------------------------
     * Pre‑compute the transition graph once.
     * States are numbered 0..48 as (x,y) → x*7 + y.
     * The adjacency matrix is stored in static arrays `nxt` and `outdeg`.
     */
    static bool initialized = false;
    static int     nxt[49][49];
    static int     outdeg[49];
    static bool    prime[25];

    if (!initialized)
    {
        /* Prime table up to 24 */
        for (int i = 0; i < 25; ++i)
            prime[i] = false;
        const int primes[] = {2,3,5,7,11,13,17,19,23};
        for (size_t i = 0; i < sizeof primes / sizeof primes[0]; ++i)
            prime[primes[i]] = true;

        /* Build adjacency lists */
        for (int s = 0; s < 49; ++s)
            outdeg[s] = 0;

        for (int a = 0; a < 7; ++a)
            for (int b = 0; b < 7; ++b)
            {
                int s = a * 7 + b;
                for (int a2 = 0; a2 < 7; ++a2)
                    for (int b2 = 0; b2 < 7; ++b2)
                    {
                        int sum = a + b + a2 + b2;
                        if (prime[sum])
                        {
                            int t = a2 * 7 + b2;
                            nxt[s][outdeg[s]++] = t;
                        }
                    }
            }
        initialized = true;
    }

    /* ------------------------------------------------------------------
     * Dynamic programming: cnt[s] = number of ways to reach state s
     * after a given number of rows.  Initially each state occurs once
     * (any first row is allowed).  Then we perform `n` transitions.
     */
    static seq_t cnt[49];
    static seq_t ncnt[49];

    for (int i = 0; i < 49; ++i)
        cnt[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step)
    {
        /* zero next counts */
        for (int i = 0; i < 49; ++i)
            ncnt[i] = (seq_t)0;

        /* propagate from each state */
        for (int s = 0; s < 49; ++s)
        {
            seq_t v = cnt[s];
            if (v == 0)
                continue;
            int deg = outdeg[s];
            for (int k = 0; k < deg; ++k)
            {
                int ns = nxt[s][k];
                ncnt[ns] += v;
            }
        }

        /* swap cnt and ncnt for next iteration */
        for (int i = 0; i < 49; ++i)
            cnt[i] = ncnt[i];
    }

    /* total number of arrays is the sum over all ending states */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 49; ++i)
        ans += cnt[i];
    return ans;
}
