/*
 * A296014  Number of n X 3 0..1 arrays with each 1 adjacent to 1 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296014: Number of n×3 0..1 arrays with each 1 adjacent to
   1 or 4 king‑move neighboring 1s.
   Recurrence:
   a(1)= 3, a(2)=13, a(3)=30, a(4)=91, a(5)=280, a(6)=785,
   a(7)=2319, a(8)=6816, a(9)=19796, a(10)=57991,
   and for n≥11
   a(n)= a(n‑1)+3·a(n‑2)+11·a(n‑3) –3·a(n‑4) –11·a(n‑5) –27·a(n‑6)
        +a(n‑7)+6·a(n‑8)+7·a(n‑9)+a(n‑10).  */

#include <stddef.h>
#include <stdlib.h>

/* Initial values, index 0 unused */
static const seq_t init_vals[11] = {
    (seq_t)0,      /* 0  – placeholder */
    (seq_t)3,      /* 1 */
    (seq_t)13,     /* 2 */
    (seq_t)30,     /* 3 */
    (seq_t)91,     /* 4 */
    (seq_t)280,    /* 5 */
    (seq_t)785,    /* 6 */
    (seq_t)2319,   /* 7 */
    (seq_t)6816,   /* 8 */
    (seq_t)19796,  /* 9 */
    (seq_t)57991   /*10*/
};

/* Static cache for memoisation */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;        /* actual allocated length  (>=1 if cache!=NULL) */
static idx_t  last_computed = 0;     /* largest index so far computed */

/* Allocate enough storage for indices 0..n inclusive */
static void ensure_capacity(idx_t n)
{
    /* Need cache_len-1 >= n  (i.e. at least n+1 elements) */
    if (cache != NULL && (cache_len - 1) >= n) return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;  /* allocation failed – caller will get 0 */

    /* initialise newly‑allocated slots to 0 */
    for (size_t i = cache_len; i < new_len; ++i) {
        tmp[i] = (seq_t)0;
    }

    cache    = tmp;
    cache_len = new_len;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for 0, but return 0 */

    ensure_capacity(n);

    /* compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[(size_t)i] = init_vals[(size_t)i];
        } else {
            seq_t val = cache[(size_t)i - 1];
            val += cache[(size_t)i - 2] * (seq_t)3;
            val += cache[(size_t)i - 3] * (seq_t)11;
            val -= cache[(size_t)i - 4] * (seq_t)3;
            val -= cache[(size_t)i - 5] * (seq_t)11;
            val -= cache[(size_t)i - 6] * (seq_t)27;
            val += cache[(size_t)i - 7];
            val += cache[(size_t)i - 8] * (seq_t)6;
            val += cache[(size_t)i - 9] * (seq_t)7;
            val += cache[(size_t)i -10];
            cache[(size_t)i] = val;
        }
    }

    last_computed = n;
    return cache[(size_t)n];
}
