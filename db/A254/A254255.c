/*
 * A254255  Number of (n+2)X(3+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two maximums of the diagonal and antidiagonal minus the sum of
 *          the minimums of the central row and column nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 30, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A254255
   Empirical recurrence (n > 19):  
        a(n) = 3*a(n-1)+6*a(n-2)-8*a(n-3)-7*a(n-4)+5*a(n-5)
               -102*a(n-6)-1232*a(n-7)+1096*a(n-8)+4880*a(n-9)
               +2464*a(n-10)-8128*a(n-11)-15360*a(n-12)+16384*a(n-13)
   Initial values: a(1)=12616, a(2)=72644, a(3)=268900, a(4)=718226,
                    a(5)=2192422, a(6)=6729896, a(7)=21120938,
                    a(8)=68298122, a(9)=216717474, a(10)=690324184,
                    a(11)=2237606074, a(12)=7136863098, a(13)=23081712162,
                    a(14)=74362141304, a(15)=239411652586,
                    a(16)=774044161546, a(17)=2505136288034,
                    a(18)=8097590302616, a(19)=26266047011098,
                    a(20)=85077718931738
   Offset: 1
   Non‑n is assumed in the definition. */

#include <stdlib.h>
#include <stdint.h>

/* Cache to store already computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 0;  /* maximum index that has been computed */

/* Ensure the cache holds indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (!cache || cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) exit(1);  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Initial 20 terms (a(0) unused) */
static const seq_t init_vals[21] = {
    0ULL,                           /* unused index 0 */
    12616ULL, 72644ULL, 268900ULL, 718226ULL,
    2192422ULL, 6729896ULL, 21120938ULL, 68298122ULL,
    216717474ULL, 690324184ULL, 2237606074ULL, 7136863098ULL,
    23081712162ULL, 74362141304ULL, 239411652586ULL,
    774044161546ULL, 2505136288034ULL, 8097590302616ULL,
    26266047011098ULL, 85077718931738ULL
};

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= 20) return init_vals[n];

    /* Prepare cache for indices up to n */
    ensure_cache(n);

    /* Initialise first 20 cache entries if not already done */
    if (cache_max < 20) {
        for (idx_t i = 1; i <= 20; ++i) cache[i] = init_vals[i];
        cache_max = 20;
    }

    /* Compute remaining terms up to n using recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 0;

        /* Positive contributions */
        val += ((seq_t)3)   * cache[i-1];
        val += ((seq_t)6)   * cache[i-2];
        val += ((seq_t)5)   * cache[i-5];
        val += ((seq_t)1096) * cache[i-8];
        val += ((seq_t)4880) * cache[i-9];
        val += ((seq_t)2464) * cache[i-10];
        val += ((seq_t)16384) * cache[i-13];

        /* Negative contributions */
        val -= ((seq_t)8)   * cache[i-3];
        val -= ((seq_t)7)   * cache[i-4];
        val -= ((seq_t)102) * cache[i-6];
        val -= ((seq_t)1232) * cache[i-7];
        val -= ((seq_t)8128) * cache[i-11];
        val -= ((seq_t)15360) * cache[i-12];

        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
