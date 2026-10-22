/*
 * A318617  a(n) is the number of rooted labeled forests on n nodes that avoid
 *          the patterns 213, 231, and 312.
 * https://oeis.org/A318617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{r=1..k}
 *            binomial(n-1,k-1)*(r-1)!*a(n-k)*a(k-r), a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_alloc.h"

static seq_t *a_arr   = NULL;
static seq_t *b_arr   = NULL;
static seq_t *fact_arr = NULL;
static idx_t  cap     = 0;
static idx_t  computed = 0;

/* Ensure the internal arrays have room for indices up to n. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cap) return;
    idx_t new_cap = n + 1;
    seq_t *new_a   = seq_alloc(new_cap);
    seq_t *new_b   = seq_alloc(new_cap);
    seq_t *new_f   = seq_alloc(new_cap);
    if (a_arr) {                /* copy old data */
        for (idx_t i = 0; i <= computed; ++i) {
            new_a[i] = a_arr[i];
            new_b[i] = b_arr[i];
            new_f[i] = fact_arr[i];
        }
    }
    a_arr   = new_a;
    b_arr   = new_b;
    fact_arr = new_f;
    cap     = new_cap;
}

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) return (seq_t)1;

    /* Allocate or grow the internal tables. */
    ensure_capacity(n);

    /* Initialise the base values if this is the first run. */
    if (computed == 0) {
        a_arr[0]    = (seq_t)1;
        b_arr[0]    = (seq_t)0;          /* unused, but set for safety */
        fact_arr[0] = (seq_t)1;
        computed   = 0;
    }

    /* Compute terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* factorial[i] = factorial[i-1] * i */
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

        /* b[i] = sum_{r=1..i} (r-1)! * a[i-r] */
        seq_t sum_b = 0;
        for (idx_t r = 1; r <= i; ++r) {
            sum_b += fact_arr[r - 1] * a_arr[i - r];
        }
        b_arr[i] = sum_b;

        /* a[i] = sum_{k=1..i} C(i-1,k-1) * a[i-k] * b[k] */
        seq_t res   = 0;
        seq_t binom = 1;                    /* C(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            res += binom * a_arr[i - k] * b_arr[k];
            if (k < i) {
                binom = (binom * (seq_t)(i - k)) / (seq_t)k;   /* C(i-1,k) */
            }
        }
        a_arr[i] = res;
        computed = i;
    }
    return a_arr[n];
}
