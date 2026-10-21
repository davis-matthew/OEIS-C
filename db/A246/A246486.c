/*
 * A246486  Number of length 7+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 10, u64 n <= 85, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A246486:
 * a(n) =  3*a(n-1) + 5*a(n-2) -23*a(n-3) -4*a(n-4) +76*a(n-5) -28*a(n-6)
 *        -140*a(n-7)+98*a(n-8)+154*a(n-9)-154*a(n-10)-98*a(n-11)+140*a(n-12)
 *        +28*a(n-13)-76*a(n-14)+4*a(n-15)+23*a(n-16)-5*a(n-17)-3*a(n-18)+a(n-19)
 *    (for n>19)
 * Initial values (a(1) … a(19)):
 *   2, 72, 6908, 87888, 1309038, 8628792, 53733080, 226048032,
 *   881180090, 2758803240, 8099880852, 20739933552, 50471282918,
 *   111972509208, 238609813808, 475857371712, 918806537970,
 *   1688132007432, 3020848830380
 */

#include <stdlib.h>
#include <stdint.h>

/* Static dynamic cache of computed terms.  cache[i] stores a(i+1). */
static seq_t *cache   = NULL;
static idx_t  cache_len = 0;   /* number of terms stored in cache */
static idx_t  cache_cap = 0;   /* capacity of cache array */

/* 19 initial terms of the sequence.  The values fit in 64‑bit integers;
 *  they are cast to seq_t so the code works for any seq_t type. */
static const seq_t init[19] = {
    (seq_t)2ULL,
    (seq_t)72ULL,
    (seq_t)6908ULL,
    (seq_t)87888ULL,
    (seq_t)1309038ULL,
    (seq_t)8628792ULL,
    (seq_t)53733080ULL,
    (seq_t)226048032ULL,
    (seq_t)881180090ULL,
    (seq_t)2758803240ULL,
    (seq_t)8099880852ULL,
    (seq_t)20739933552ULL,
    (seq_t)50471282918ULL,
    (seq_t)111972509208ULL,
    (seq_t)238609813808ULL,
    (seq_t)475857371712ULL,
    (seq_t)918806537970ULL,
    (seq_t)1688132007432ULL,
    (seq_t)3020848830380ULL
};

/* 19 recurrence coefficients c[0] … c[18] such that
 * a(n) = Σ_{k=0}^{18} c[k] * a(n-k-1).                           */
static const int coeff[19] = {
    3, 5, -23, -4, 76, -28, -140, 98, 154,
   -154, -98, 140, 28, -76, 4, 23, -5, -3, 1
};

/* Expand the cache so that it contains terms a(1) … a(n). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;  /* already computed */

    /* Extend capacity if needed. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 256;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i < 19) {          /* Initial part */
            cache[i] = init[i];
        } else {               /* Recurrence */
            seq_t pos = 0;    /* sum of positive contributions */
            seq_t neg = 0;    /* sum of absolute values of negative contributions */

            for (int k = 0; k < 19; ++k) {
                seq_t term = cache[i - k - 1];
                int c = coeff[k];
                if (c > 0)
                    pos += term * (seq_t)c;
                else
                    neg += term * (seq_t)(-c);
            }
            cache[i] = pos - neg;   /* a(i+1) = pos - neg */
        }
    }
    cache_len = n;
}

/* External function required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1 */

    ensure_cache(n);
    return cache[n - 1];   /* cache index 0 corresponds to a(1) */
}
