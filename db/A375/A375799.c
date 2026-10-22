/*
 * A375799  Expansion of e.g.f. 1/(1 + (log(1 - x^3))/x^2).
 * https://oeis.org/A375799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (n-3k)! * |Stirling1(n-2k, n-3k)|/(n-2k)!
 * Recurrence used: for n ≥ 3
 *   a(n) = Σ_{m=1..⌊(n+2)/3⌋} (factorial[n] / (m * factorial[n-3m+2])) * a[n-3m+2]
 *   (The quotient factorial[n]/factorial[n-3m+2] is always divisible by m, so the
 *    division is exact and the sum stays integer.)
 *
 * This implementation memoises factorials and values of a(n) up to the largest
 * index requested.  It works for any seq_t and idx_t supplied by the build.
 */
#include <stdlib.h>
#include <stddef.h>

/* Memoised computation of A375799 */
seq_t a(idx_t n)
{
    static idx_t  max_n = 0;          /* highest n for which arrays are initialised */
    static idx_t  cap   = 0;          /* current array capacity (>= max_n+1) */
    static seq_t *fact  = NULL;       /* factorials 0! … n! */
    static seq_t *arr   = NULL;       /* a(n) values */

    /* Allocate or grow storage if necessary */
    if (cap <= n) {
        idx_t new_cap = (idx_t)(n + 1);
        seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        seq_t *new_arr  = (seq_t *)realloc(arr,  new_cap * sizeof(seq_t));
        if (!new_fact || !new_arr) return (seq_t)0;   /* allocation failed */
        fact = new_fact;
        arr  = new_arr;
        cap  = new_cap;
    }

    /* Initialise factorials and base cases on first use */
    if (max_n == 0) {
        fact[0] = (seq_t)1;
        arr[0]  = (seq_t)1;          /* a(0) */
        /* ensure we have space for n=1,2 */
        if (cap < 3) {
            idx_t new_cap = 3;
            seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            seq_t *new_arr  = (seq_t *)realloc(arr,  new_cap * sizeof(seq_t));
            if (!new_fact || !new_arr) return (seq_t)0;
            fact = new_fact;
            arr  = new_arr;
            cap  = new_cap;
        }
        fact[1] = fact[0] * (seq_t)1;
        arr[1]  = (seq_t)1;          /* a(1) */
        fact[2] = fact[1] * (seq_t)2;
        arr[2]  = (seq_t)2;          /* a(2) */
        max_n   = 2;
    }

    /* Extend precomputation up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* factorial[ cur ] = factorial[ cur-1 ] * cur */
        fact[cur] = fact[cur - 1] * (seq_t)cur;

        seq_t sum = (seq_t)0;
        for (idx_t m = (idx_t)1; ; ++m) {
            idx_t k = 3 * m - 2;                 /* degree of term in A(x)   */
            if (k > cur) break;                  /* no further contributions */
            idx_t idx = cur - 3 * m + 2;          /* n-3m+2, always ≥ 0      */

            /* product_j = factorial[cur] / factorial[idx]  (exact integer) */
            seq_t prod = fact[cur] / fact[idx];

            /* term = (prod / m) * a[idx]                   */
            seq_t term = (prod / (seq_t)m) * arr[idx];
            sum += term;
        }
        arr[cur] = sum;
    }

    max_n = n;
    return arr[n];
}
