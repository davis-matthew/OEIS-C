/*
 * A371210  Number of minimum vertex colorings in the complement of the path
 *          graph on n nodes.
 * https://oeis.org/A371210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if (n even) (n/2)! else ((n+1)/2)*((n+1)/2)!
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1, a(3)=4, a(4)=2, ...  (as in OEIS A371210) */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *fact_arr = NULL;   /* array of factorials fact_arr[i] = i! */
static idx_t fact_max = (idx_t)-1; /* largest k for which fact_arr[k] is computed */
static size_t fact_cap = 0;        /* allocated capacity of fact_arr */

/* Ensure fact_arr has capacity for at least n+1 elements. */
static void ensure_fact_capacity(idx_t n) {
    if ((idx_t)fact_cap > n) return;
    size_t new_cap = fact_cap == 0 ? 4 : fact_cap * 2;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    fact_arr = new_arr;
    fact_cap = new_cap;
}

/* Ensure factorials are computed up to n. */
static void compute_factorials(idx_t n) {
    if (fact_arr == NULL) {
        /* first time: allocate and set 0! = 1 */
        fact_cap = 4;
        fact_arr = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact_arr[0] = (seq_t)1;
        fact_max = 0;
    }
    if ((idx_t)fact_cap <= n) ensure_fact_capacity(n);
    if (fact_max < n) {
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact_arr[i] = fact_arr[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }
}

seq_t a(idx_t n) {
    /* n >= 1 by definition of the sequence */
    if (n & 1ULL) {          /* odd n: k = (n+1)/2 */
        idx_t k = (n + 1) >> 1;
        compute_factorials(k);
        return fact_arr[k] * (seq_t)k;   /* factor k * k! */
    } else {                 /* even n: k = n/2 */
        idx_t k = n >> 1;
        compute_factorials(k);
        return fact_arr[k];
    }
}
