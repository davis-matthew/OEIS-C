/*
 * A396729  Irregular triangle read by rows in which row n lists the first
 *          A000041(n) positive integers, n >= 0.
 * https://oeis.org/A396729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, new
 * Formula:   a(n) = (n - S(r-1)) + 1 where r is the minimal index such that
 *            S(r) = Σ_{k=0}^{r} A000041(k) > n. A000041(k) is the partition
 *            function p(k). The triangle is formed by writing the first p(k)
 *            positive integers in row k
 * Range:     u8 n <= 96, u16 n <= 9586, u32 n <= 9586, u64 n <= 9586,
 *            u128 n <= 9586, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* (1) Container for partition numbers p(k) and cumulative sums
       S(k) = Σ_{i=0}^{k} p(i).  They are grown lazily on the first
       call for each increasing n.  */
    static seq_t *p = NULL;
    static seq_t *cum = NULL;
    static size_t p_len = 0;   /* current allocated elements for p */
    static size_t cum_len = 0; /* current allocated elements for cum */
    static size_t computed = 0;/* number of p values computed */

    /* (2) Ensure we have space for at least one element.  */
    if (p_len == 0) {
        p_len = 16;
        p = (seq_t *)malloc(p_len * sizeof(seq_t));
        cum = (seq_t *)malloc(p_len * sizeof(seq_t));
        /* Initialize first partition p(0)=1.  */
        p[0] = (seq_t)1;
        cum[0] = (seq_t)1;
        computed = 1;
        cum_len = 1;
    }

    /* (3) Grow partition and cumulative arrays until bisect
       cum[computed-1] > n, i.e. we have computed row r.  */
    while (cum_len == 0 || cum[cum_len - 1] <= n) {
        /* Ensure capacity.  */
        if (computed >= p_len) {
            size_t new_len = p_len * 2;
            p = (seq_t *)realloc(p, new_len * sizeof(seq_t));
            cum = (seq_t *)realloc(cum, new_len * sizeof(seq_t));
            p_len = new_len;
        }

        /* Compute p(computed) using the pentagonal number theorem.  */
        idx_t i = computed;   /* new index */
        long long sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i)
                break;
            if (k & 1)
                sum += (long long)p[i - g1];
            else
                sum -= (long long)p[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 > i)
                continue;
            if (k & 1)
                sum += (long long)p[i - g2];
            else
                sum -= (long long)p[i - g2];
        }
        p[i] = (seq_t)sum;

        /* Update cumulative sum.  */
        seq_t prev_cum = (i == 0) ? (seq_t)0 : cum[i - 1];
        cum[i] = prev_cum + p[i];

        ++computed;
        ++cum_len;
    }

    /* (4) Find the row r where cum[r] > n, by linear scan (cum_len
         is at most a few dozen for n up to 10,000).  */
    idx_t r = 0;
    while (r < cum_len && cum[r] <= n)
        ++r;

    /* (5) Position inside the row (0‑based).  */
    idx_t prev_cum = (r == 0) ? (idx_t)0 : (idx_t)cum[r - 1];
    idx_t pos = n - prev_cum;

    /* (6) The triangle always starts with 1, so the value is pos+1.  */
    return (seq_t)(pos + 1);
}
