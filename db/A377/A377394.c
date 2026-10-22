/*
 * A377394  Expansion of e.g.f. (1 - log(1-x))^3.
 * https://oeis.org/A377394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377394 – Expansion of e.g.f. (1 - log(1-x))^3
 *
 * Formula:  a(n) = Σ_{k=0..3} k! * binomial(3,k) * |Stirling1(n,k)|
 *   (Stirling1 – unsigned Stirling numbers of the first kind)
 *
 * The implementation builds the four Stirling numbers of the first kind
 * for k = 0..3 by the recurrence
 *
 *   s(0,0)=1 ,  s(0,k>0)=0
 *   s(n,0)=0   (n>0)
 *   s(n,k)=s(n-1,k-1)+(n-1)*s(n-1,k)  (1≤k≤3)
 *
 * For each n the sequence value is
 *
 *   a(n) = s(n,0) + 3·s(n,1) + 6·s(n,2) + 6·s(n,3)
 *
 * The routine caches all computed values so that calls in arbitrary
 * order are fast.  It works for any supported seq_t (fixed‑size or
 * arbitrary‑precision) and any idx_t (unsigned 64‑bit).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  last_n = 0;

    /* Stirling numbers s(n,k) for 0≤k≤3 */
    static seq_t s0 = 1, s1 = 0, s2 = 0, s3 = 0;

    /* First call – initialise the cache */
    if (len == 0) {
        cap   = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        len    = 1;
        last_n = 0;
        s0 = 1;  s1 = 0;  s2 = 0;  s3 = 0;
    }

    /* Fast return if already cached */
    if ((size_t)n < len)
        return cache[n];

    /* Grow the cache to fit index n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap   = new_cap;
    }

    /* Extend computation from last_n+1 up to n */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        seq_t ns0 = 0; /* s(m,0)=0 for m>0 */
        seq_t ns1 = s0 + ((seq_t)(m - 1)) * s1;
        seq_t ns2 = s1 + ((seq_t)(m - 1)) * s2;
        seq_t ns3 = s2 + ((seq_t)(m - 1)) * s3;

        /* a(m) = s0 + 3*s1 + 6*s2 + 6*s3 */
        seq_t val = ns0 + (seq_t)3 * ns1 + (seq_t)6 * ns2 + (seq_t)6 * ns3;

        cache[m] = val;

        /* Prepare for next iteration */
        s0 = ns0; s1 = ns1; s2 = ns2; s3 = ns3;
    }

    len    = (size_t)n + 1;
    last_n = n;
    return cache[n];
}
