/*
 * A367884  Expansion of e.g.f. 1/(1 + x * log(1-3*x)).
 * https://oeis.org/A367884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A367884: Expansion of e.g.f. 1/(1 + x * log(1-3*x));
 * a(0) = 1, a(1) = 0,
 * a(n) = n! * Σ_{k=2..n} 3^(k-1)/(k-1) * a(n-k)/(n-k)!   (n ≥ 2)   */
   
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *fact_arr   = NULL;   /* factorials 0!..n!   */
static seq_t *pow3_arr   = NULL;   /* powers of 3      3^0..3^n   */
static seq_t *a_arr      = NULL;   /* sequence values   */
static size_t cap = 0;             /* current array capacity */

/* ensure arrays are large enough for indices up to needed-1 */
static void ensure_cap(size_t needed) {
    if (needed <= cap) return;
    size_t new_cap = (cap > 0) ? (cap << 1) : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp;
    tmp = (seq_t *)realloc(fact_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    fact_arr = tmp;
    tmp = (seq_t *)realloc(pow3_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    pow3_arr = tmp;
    tmp = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_arr = tmp;
    cap = new_cap;
}

/* compute the sequence up to n inclusive */
static void compute_up_to(idx_t n) {
    size_t N = (size_t)n;
    /* allocate storage */
    ensure_cap(N + 1);

    /* initialise base cases once */
    if (N >= 0 && a_arr[0] == (seq_t)0) {
        a_arr[0] = (seq_t)1;
        fact_arr[0] = (seq_t)1;
        pow3_arr[0] = (seq_t)1;
    }
    if (N >= 1 && a_arr[1] == (seq_t)0 && fact_arr[1] == (seq_t)0 && pow3_arr[1] == (seq_t)0) {
        a_arr[1] = (seq_t)0;
        fact_arr[1] = fact_arr[0] * (seq_t)1;
        pow3_arr[1] = pow3_arr[0] * (seq_t)3;
    }

    /* fill factorials and powers of 3 */
    for (size_t i = 2; i <= N; ++i) {
        if (fact_arr[i] == (seq_t)0)
            fact_arr[i] = fact_arr[i-1] * (seq_t)i;
    }
    for (size_t i = 2; i <= N; ++i) {
        if (pow3_arr[i] == (seq_t)0)
            pow3_arr[i] = pow3_arr[i-1] * (seq_t)3;
    }

    /* compute a(n) for n >= 2 */
    for (size_t i = 2; i <= N; ++i) {
        if (a_arr[i] == (seq_t)0) {   /* not yet computed */
            seq_t sum = (seq_t)0;
            for (idx_t k = 2; k <= (idx_t)i; ++k) {
                /* term = fact[i] / (fact[i-k] * (k-1)) * a[i-k] * 3^(k-1) */
                seq_t term = fact_arr[i];
                term = term / fact_arr[i-k];
                term = term / (seq_t)(k-1);
                term = term * a_arr[i-k];
                term = term * pow3_arr[k-1];
                sum += term;
            }
            a_arr[i] = sum;
        }
    }
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    compute_up_to(n);
    return a_arr[N];
}
