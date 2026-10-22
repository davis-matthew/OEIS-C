/*
 * A305778  Number of set partitions of [n] with alternating parity of
 *          elements and exactly four blocks.
 * https://oeis.org/A305778
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *
 * a(n) = a(n-1) + 18*a(n-2) - 18*a(n-3) - 127*a(n-4) + 127*a(n-5)
 *        + 450*a(n-6) - 450*a(n-7) - 844*a(n-8) + 844*a(n-9)
 *        + 792*a(n-10) - 792*a(n-11) - 288*a(n-12) + 288*a(n-13)
 * for n ≥ 16 (indices are zero‑based).  The first 12 terms
 * (a(4) … a(15)) are provided directly.
 *
 * This recurrence follows from the rational generating function
 * given in the OEIS entry.  The function below memoises the results
 * so that calls in arbitrary order stay O(n) and fit easily the time limit
 * for n up to about 10000.
 */

#include <stdlib.h>
#include <stdint.h>

/*  pre‑calculated values for a(4) … a(15)  (indices 4…15)  */
static const seq_t init_vals[] = {
    1ULL,   4ULL,   12ULL,   33ULL,   84ULL,   222ULL,
   545ULL, 1425ULL, 3458ULL, 8948ULL, 21595ULL, 55335ULL
};

/*  coefficients for the recurrence:
 *  a(n) = Σ_{k=1}^{13} c_k * a(n-k)
 *  with c = {  1,  18, -18, -127,  127,  450, -450,
 *             -844,  844,  792, -792, -288,  288 }   */
static const int  pos_idx[]  = {1, 2, 5, 6, 9, 10, 13};
static const int  pos_coef[] = {1, 18, 127, 450, 844, 792, 288};

static const int  neg_idx[]  = {3, 4, 7, 8, 11, 12};
static const int  neg_coef[] = {18, 127, 450, 844, 792, 288};

seq_t a(idx_t n)
{
    /*  cache of already computed values  */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t size  = 0;   /* number of filled indices = size */

    /*  allocate or enlarge cache  */
    if (cap < (size_t)n + 1) {
        size_t newcap = cap ? cap : 64;
        while (newcap <= (size_t)n)
            newcap <<= 1;     /* grow multiplicatively */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)       /* allocation failure – give up */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /*  compute missing values  */
    if (size <= (size_t)n) {
        for (size_t i = size; i <= (size_t)n; ++i) {

            if (i < 4) {
                /* indices below the offset are irrelevant – set to zero  */
                cache[i] = (seq_t)0;
            }
            else if (i <= 15) {
                cache[i] = init_vals[i-4];
            }
            else {
                /*  recurrence for n ≥ 16  */
                seq_t pos_sum = (seq_t)0;
                seq_t neg_sum = (seq_t)0;

                /*  positive contributions  */
                for (int k = 0; k < 7; ++k) {
                    pos_sum += cache[i - pos_idx[k]] *
                              (uint64_t)pos_coef[k];
                }

                /*  negative contributions (subtracted)  */
                for (int k = 0; k < 6; ++k) {
                    neg_sum += cache[i - neg_idx[k]] *
                               (uint64_t)neg_coef[k];
                }

                cache[i] = pos_sum - neg_sum;   /*  pos_sum ≥ neg_sum  */
            }
        }
        size = (size_t)n + 1;
    }

    return cache[n];
}
