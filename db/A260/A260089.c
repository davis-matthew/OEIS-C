/*
 * A260089  Expansion of psi(x^2) * f(x, x^2) in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A260089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Computation of OEIS A260089:  
   a(0) = 1; for n ≥ 1  
   a(n) = (1/n) * Σ_{k=1..n} c_k * a(n‑k),  
   where c_k = Σ_{d | k} d * s_d and the periodic sequence  
   s_d has period 12 given by    
   [1, 1, -1, -1, 1, 0, 1, -1, -1, 1, 1, -2]   (for d = 1,2,…).  
   This is the Euler transform of the specified periodic sequence.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;   /* cache of a[0 .. a_len-1]          */
    static int   *s_cache = NULL;   /* cache of s[0 .. s_len-1]          */
    static int   *c_cache = NULL;   /* cache of c[0 .. c_len-1]          */
    static size_t a_len = 0;        /* number of a values stored         */
    static size_t s_len = 0;        /* maximum index computed for s      */
    static size_t c_len = 0;        /* maximum index computed for c      */

    /* Periodic values of s_d for d ≥ 1 (index 0 unused) */
    const int period[12] = { 1, 1, -1, -1, 1, 0, 1, -1, -1, 1, 1, -2 };
    size_t req = (size_t)n + 1;     /* we need values up to index n      */

    /* Extend and compute s_cache[1..req-1] if needed */
    if (s_len < req)
    {
        int *tmp = (int *)realloc(s_cache, req * sizeof(int));
        if (!tmp) return 0;            /* allocation failed */
        s_cache = tmp;
        for (size_t i = s_len; i < req; ++i)
        {
            if (i == 0) continue;      /* s[0] is unused */
            s_cache[i] = period[(i - 1) % 12];
        }
        s_len = req;
    }

    /* Extend and compute c_cache[1..req-1] if needed */
    if (c_len < req)
    {
        int *tmp = (int *)realloc(c_cache, req * sizeof(int));
        if (!tmp) return 0;
        c_cache = tmp;
        /* initialize all to zero */
        for (size_t i = 0; i < req; ++i) c_cache[i] = 0;
        /* compute c_k = Σ_{d|k} d * s_d */
        for (size_t d = 1; d < req; ++d)
        {
            int sd = s_cache[d];
            if (sd == 0) continue;
            for (size_t k = d; k < req; k += d)
                c_cache[k] += (int)(d * sd);
        }
        c_len = req;
    }

    /* Extend and compute a_cache[0..req-1] if needed */
    if (a_len < req)
    {
        seq_t *tmp = (seq_t *)realloc(a_cache, req * sizeof(seq_t));
        if (!tmp) return 0;
        a_cache = tmp;
        if (a_len == 0)
        {
            a_cache[0] = 1;             /* a(0) = 1 */
            a_len = 1;
        }
        for (size_t i = a_len; i < req; ++i)
        {
            seq_t sum = 0;              /* seq_t may hold signed values */
            for (size_t k = 1; k <= i; ++k)
                sum += a_cache[i - k] * (seq_t)c_cache[k];
            a_cache[i] = sum / (seq_t)i; /* division is exact by construction */
        }
        a_len = req;
    }

    return a_cache[(size_t)n];
}
