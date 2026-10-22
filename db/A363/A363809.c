/*
 * A363809  Number of permutations of [n] that avoid the patterns 2-41-3,
 *          3-14-2, and 2-1-3-5-4.
 * https://oeis.org/A363809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The coefficients a(n) of the generating function * satisfy the
 *            algebraic equation * x^4*(x-2)^2*F^4 +
 *            x*(x-2)*(4*x^3-7*x^2+6*x-1)*F^3 * + (2*x^4-x^3-2*x^2+5*x-1)*F^2
 *            * - (4*x^3-7*x^2+6*x-1)*F + x^2 = 0 . * From this we derive a
 *            direct recurrence: * a(0)=1, a(1)=1, a(2)=2, * for n≥3 * a(n) =
 *            Pos(n...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *a_arr  = NULL;
static seq_t *c2_arr = NULL;
static seq_t *c3_arr = NULL;
static seq_t *c4_arr = NULL;
static idx_t   cap   = 0;   /* allocated size of arrays */
static idx_t   cached_n = 0;/* last index that has been computed */

static void grow_arrays(idx_t new_cap)
{
    if (new_cap <= cap) return;
    a_arr  = (seq_t*)realloc(a_arr,  new_cap * sizeof(seq_t));
    c2_arr = (seq_t*)realloc(c2_arr, new_cap * sizeof(seq_t));
    c3_arr = (seq_t*)realloc(c3_arr, new_cap * sizeof(seq_t));
    c4_arr = (seq_t*)realloc(c4_arr, new_cap * sizeof(seq_t));
    /* zero new entries */
    for (idx_t i = cap; i < new_cap; ++i)
    {
        a_arr[i]  = 0;
        c2_arr[i] = 0;
        c3_arr[i] = 0;
        c4_arr[i] = 0;
    }
    cap = new_cap;
}

static void init_base(void)
{
    /* ensure space for first three terms */
    grow_arrays(3);

    a_arr[0] = 1;
    a_arr[1] = 1;
    a_arr[2] = 2;

    /* c2[0] = a0*a0 */
    c2_arr[0] = (seq_t)1;
    /* c2[1] = a0*a1 + a1*a0 = 2 */
    c2_arr[1] = (seq_t)2;
    /* c2[2] = a0*a2 + a1*a1 + a2*a0 = 5 */
    c2_arr[2] = (seq_t)5;

    /* c3[0] = a0^3 */
    c3_arr[0] = (seq_t)1;
    /* c3[1] = 3 * a0*a1*a0 = 3 */
    c3_arr[1] = (seq_t)3;
    /* c3[2] = 9 */
    c3_arr[2] = (seq_t)9;

    /* c4[0] = a0^4 */
    c4_arr[0] = (seq_t)1;
    /* c4[1] = 4 */
    c4_arr[1] = (seq_t)4;
    /* c4[2] = 14 */
    c4_arr[2] = (seq_t)14;

    cached_n = 2; /* we have computed up to index 2 */
}

static inline seq_t get_c2(idx_t i) { return (i <= cached_n ? c2_arr[i] : (seq_t)0); }
static inline seq_t get_c3(idx_t i) { return (i <= cached_n ? c3_arr[i] : (seq_t)0); }
static inline seq_t get_c4(idx_t i) { return (i <= cached_n ? c4_arr[i] : (seq_t)0); }

seq_t a(idx_t n)
{
    if (cap == 0) init_base();

    if (n < cached_n) {
        /* value already computed */
        return a_arr[n];
    }

    if (n <= 2) {
        /* base cases */
        return a_arr[n];
    }

    /* extend cache up to n */
    grow_arrays(n + 1); /* ensure n is allocatable */

    for (idx_t k = cached_n + 1; k <= n; ++k) {
        /* --- compute sum_i1 = Σ_{i=1}^{k-1} a[i]*a[k-i] --- */
        seq_t sum_i1 = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum_i1 += a_arr[i] * a_arr[k - i];
        }

        /* --- compute positive part of recurrence --- */
        seq_t pos = 0;
        if (k >= 6) pos += get_c4(k - 6);
        if (k >= 4) pos += (seq_t)4 * get_c4(k - 4);
        if (k >= 5) pos += (seq_t)4 * get_c3(k - 5);
        if (k >= 3) pos += (seq_t)20 * get_c3(k - 3);
        if (k >= 1) pos += (seq_t)2 * get_c3(k - 1);
        if (k >= 4) pos += (seq_t)2 * get_c2(k - 4);
        if (k >= 1) pos += (seq_t)5 * get_c2(k - 1);
        if (k == 2) pos += (seq_t)1; /* term5 */

        /* --- compute negative part --- */
        seq_t neg = 0;
        if (k >= 5) neg += (seq_t)4 * get_c4(k - 5);
        if (k >= 4) neg += (seq_t)15 * get_c3(k - 4);
        if (k >= 2) neg += (seq_t)13 * get_c3(k - 2);
        if (k >= 3) neg += get_c2(k - 3);
        if (k >= 2) neg += (seq_t)2 * get_c2(k - 2);
        neg += sum_i1;

        /* part1 = 4*a[k-3] + 6*a[k-1] - 7*a[k-2] (positive) */
        seq_t part1 = (seq_t)4 * a_arr[k - 3];
        part1 += (seq_t)6 * a_arr[k - 1];
        part1 -= (seq_t)7 * a_arr[k - 2]; /* subtraction below guaranteed non-negative */
        neg += part1;

        /* --- finally compute a[k] --- */
        a_arr[k] = pos - neg;

        /* --- update convolution arrays for future steps --- */
        /* c2[k] = 2*a[k] + sum_i1 */
        c2_arr[k] = (seq_t)2 * a_arr[k] + sum_i1;

        /* c3[k] = Σ_{i=0}^{k} a[i] * c2[k-i] */
        seq_t c3k = 0;
        for (idx_t i = 0; i <= k; ++i) {
            c3k += a_arr[i] * c2_arr[k - i];
        }
        c3_arr[k] = c3k;

        /* c4[k] = Σ_{i=0}^{k} a[i] * c3[k-i] */
        seq_t c4k = 0;
        for (idx_t i = 0; i <= k; ++i) {
            c4k += a_arr[i] * c3_arr[k - i];
        }
        c4_arr[k] = c4k;
    }

    cached_n = n;
    return a_arr[n];
}
