/*
 * A285363  Sum of the entries in the first blocks of all set partitions of
 *          [n].
 * https://oeis.org/A285363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B_n + B_{n-1} * (n*(n+1)/2 - 1) where B_k denotes the
 *            k‑th Bell number
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- include required headers ---------- */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t, etc. */

/* ---------- helper function prototypes that may be supplied by the build ----------
   The implementation relies only on the following utilities.

   seq_t binom(idx_t a, idx_t b);           // binomial coefficient C(a,b)

   Since the build may provide a custom bigint type for seq_t, all arithmetic
   operations (+, -, *, /, %) are defined for it.  The idx_t type is an
   unsigned 64‑bit integer.  No other utilities are used.
   -------------------------------------------------------------- */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /*  static cache of Bell numbers: bell[k] holds B_k   */
    static seq_t *bell = NULL;    /* dynamic array of Bell numbers */
    static idx_t bell_capacity = 0;  /* allocated capacity in slots   */
    static idx_t bell_len      = 0;  /* number of computed entries     */

    /*  safety: if the function is asked for n < 1, return 0 (offset is 1) */
    if (n < 1)
        return 0;

    /* ---------- initialise the cache if needed ---------- */
    if (bell_len == 0) {
        bell_capacity = 4;  /* initial capacity */
        bell = (seq_t *)malloc(bell_capacity * sizeof(seq_t));
        if (bell == NULL)   /* allocation failure: return 0 */
            return 0;
        bell[0] = 1;        /* B_0 = 1                           */
        bell_len = 1;
    }

    /* ---------- grow the cache to fit requested n ---------- */
    if (n + 1 > bell_capacity) {          /* need more space           */
        idx_t new_cap = bell_capacity * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failure         */
            return 0;
        bell = tmp;
        bell_capacity = new_cap;
    }

    /* ---------- compute Bell numbers up to n (if not yet computed) ---------- */
    if (n >= bell_len) {
        for (idx_t i = bell_len; i <= n; ++i) {     /* compute B_i           */
            seq_t sum = 0;
            if (i == 0) {                          /* i==0 handled before */
                bell[i] = 1;
                continue;
            }
            /* compute binom(i-1, k) incrementally */
            seq_t coeff = 1;                       /* C(i-1, 0) == 1        */
            for (idx_t k = 0; k < i; ++k) {
                sum += coeff * bell[k];
                if (k + 1 < i) {                   /* update coeff for next */
                    coeff = coeff * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
                }
            }
            bell[i] = sum;
        }
        bell_len = n + 1;   /* now we have B_0 … B_n          */
    }

    /* ---------- combine Bell numbers to obtain a(n) ---------- */
    seq_t sum_first = ((seq_t)n * ((seq_t)n + 1) / 2) - 1;   /* n*(n+1)/2 - 1 */
    seq_t result = bell[n] + bell[n - 1] * sum_first;

    return result;
}
