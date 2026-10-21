/*
 * A264656  Number of n X 1 arrays of permutations of 0..n*1-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 5.
 * https://oeis.org/A264656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..4} ( ⌊(n+i)/5⌋! )
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 35, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code uses only the allowed arithmetic operators and a static
   cache of factorials.  It works for any seq_t (fixed‑size or
   arbitrary‑precision) and any idx_t. */
#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* fact_cache[k] == k! for 0 ≤ k < fact_cache_size */
static idx_t  fact_cache_size = 0;

/* Ensure the factorial table contains factorials up to m */
static void ensure_fact(idx_t m)
{
    if (fact_cache_size <= m) {
        size_t new_size = (size_t)(m + 1);
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp)      /* allocation failed – leave cache unchanged */
            return;

        if (fact_cache_size == 0)
            tmp[0] = 1;                  /* 0! = 1 */

        for (idx_t k = fact_cache_size; k <= m; ++k) {
            if (k == 0)
                tmp[k] = 1;              /* 0! already handled */
            else
                tmp[k] = tmp[k - 1] * (seq_t)k;
        }

        fact_cache = tmp;
        fact_cache_size = new_size;
    }
}

/* OEIS sequence A264656 */
seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 0; i < 5; ++i) {
        idx_t m = (n + i) / 5;          /* floor((n+i)/5) */
        ensure_fact(m);
        result = result * fact_cache[m];
    }
    return result;
}
