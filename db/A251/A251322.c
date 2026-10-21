/*
 * A251322  Number of (n+1)X(4+1) 0..1 arrays with every 2X2 subblock having
 *          one or two 1s.
 * https://oeis.org/A251322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A251322:  Number of (n+1)×5 binary arrays with every 2×2 sub‑block
    having exactly 1 or 2 ones.
    Computed by a dynamic programming over the 32 possible 5‑bit row
    patterns.  A 32×32 transition matrix M[p][q] is true iff rows p and q
    are compatible with the 2×2 rule.  The DP vector dp[k][p] counts
    arrays of height k+1 that end with row pattern p.  Then

        a(n) = Σ_p dp[n‑1][p]        (n ≥ 1)

    The function stores previously computed terms in a static cache so
    that calls with arbitrary indices up to about 10 000 take O(n·32²)
    time.  The code works for both C and C++ builds; if seq_t is an
    arbitrary-precision integer class then the compiler creates and
    destroys objects safely.  */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <cstddef>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#endif

/* --- helpers for allocating a seq_t array in C and C++ ------------------- */
#if defined(__cplusplus)

static seq_t *realloc_seq(seq_t *p, size_t oldlen, size_t newlen)
{
    seq_t *q = new seq_t[newlen];
    for (size_t i = 0; i < oldlen; ++i) q[i] = p[i];
    delete[] p;
    return q;
}
#else

static seq_t *realloc_seq(seq_t *p, size_t oldlen, size_t newlen)
{
    (void)oldlen; /* unused in C */
    return (seq_t *)realloc(p, newlen * sizeof(seq_t));
}
#endif

/* --- main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state shared between calls */
    static bool init = false;
    static bool allowed[32][32];
    static seq_t dp_cur[32];
    static seq_t dp_next[32];
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* allocated size of cache */
    static size_t cur_max  = 0;     /* highest index computed, a(0) unused */

    /* --- initialise the transition matrix and first row DP --- */
    if (!init)
    {
        for (int p = 0; p < 32; ++p)
        {
            for (int q = 0; q < 32; ++q)
            {
                bool ok = true;
                for (int col = 0; col <= 3; ++col)
                {
                    int sum =
                        ((p >> col) & 1) + ((p >> (col + 1)) & 1) +
                        ((q >> col) & 1) + ((q >> (col + 1)) & 1);
                    if (sum != 1 && sum != 2)
                    { ok = false; break; }
                }
                allowed[p][q] = ok;
            }
        }
        for (int i = 0; i < 32; ++i) dp_cur[i] = 1;  /* first row arbitrary */
        init = true;
    }

    if (n == 0) return 0;   /* a(0) is unused, offset is 1 */

    /* --- extend cache up to requested n ----------------------- */
    while (cur_max < n)
    {
        /* compute next DP from current */
        for (int i = 0; i < 32; ++i) dp_next[i] = 0;
        for (int p = 0; p < 32; ++p)
        {
            seq_t val = dp_cur[p];
            if (!val) continue;
            for (int q = 0; q < 32; ++q)
                if (allowed[p][q]) dp_next[q] += val;
        }

        /* sum dp_next to get a(cur_max+1) */
        seq_t sum = 0;
        for (int i = 0; i < 32; ++i) sum += dp_next[i];

        /* ensure cache capacity */
        if (cache_len <= cur_max + 1)
        {
            size_t new_len = (cache_len == 0) ? 4 : cache_len;
            while (new_len <= cur_max + 1) new_len <<= 1;
            cache = realloc_seq(cache, cache_len, new_len);
            cache_len = new_len;
        }
        cache[cur_max + 1] = sum;
        ++cur_max;

        /* advance DP */
        for (int i = 0; i < 32; ++i) dp_cur[i] = dp_next[i];
    }

    return cache[n];
}
