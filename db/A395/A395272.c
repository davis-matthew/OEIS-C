/*
 * A395272  a(n) = (2*n)! * [x^(2*n)] cos(2*x) * cos(5*x) / cos(10*x).
 * https://oeis.org/A395272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * (49^n + 9^n)/2 - Sum_{k=0..n-1} (-100)^(n-k) *
 *            binomial(2*n,2*k) * a(k) (OEIS A395272)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"    /* binomial(idx_t, idx_t) */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t, seq_t) */

/* ---------- power tables ---------- */
static seq_t *pow49_arr = NULL;
static seq_t *pow9_arr  = NULL;
static seq_t *pow100_arr = NULL;
static idx_t  pow_cap = 0;

static void ensure_pow_tables(idx_t n)
{
    if (pow_cap > n) return;

    idx_t new_cap = n + 1;
    seq_t *p49 = (seq_t *)realloc(pow49_arr, new_cap * sizeof(seq_t));
    seq_t *p9  = (seq_t *)realloc(pow9_arr,  new_cap * sizeof(seq_t));
    seq_t *p100 = (seq_t *)realloc(pow100_arr, new_cap * sizeof(seq_t));
    if (!p49 || !p9 || !p100) abort();
    pow49_arr = p49;  pow9_arr = p9;  pow100_arr = p100;

    if (pow_cap == 0) {
        pow49_arr[0] = (seq_t)1;
        pow9_arr[0]  = (seq_t)1;
        pow100_arr[0] = (seq_t)1;
        pow_cap = 1;
    }
    for (idx_t i = pow_cap; i <= n; ++i) {
        pow49_arr[i]  = seq_mul(pow49_arr[i-1], (seq_t)49);
        pow9_arr[i]   = seq_mul(pow9_arr[i-1], (seq_t)9);
        pow100_arr[i] = seq_mul(pow100_arr[i-1], (seq_t)100);
    }
    pow_cap = new_cap;
}

/* ---------- memoised a(n) ---------- */
static seq_t *a_cache = NULL;
static idx_t  a_cap   = 0;

static void ensure_a_cache(idx_t n)
{
    if (n < a_cap) return;

    idx_t new_cap = n + 1;
    seq_t *arr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!arr) abort();
    a_cache = arr;

    if (a_cap == 0) {
        a_cache[0] = (seq_t)1;  /* a(0) = 1 */
        a_cap = 1;
    }

    for (idx_t i = a_cap; i <= n; ++i) {
        /* first term: (-1)^i * (49^i + 9^i)/2 */
        seq_t Btemp = (pow49_arr[i] + pow9_arr[i]) / (seq_t)2;
        seq_t a_i = ((i & 1) ? -Btemp : Btemp);

        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            idx_t e = i - k;                         /* power of -100 */
            seq_t term = seq_mul(pow100_arr[e], binomial(2*i, 2*k));
            if (e & 1) term = -term;                 /* (-100)^e */
            term = seq_mul(term, a_cache[k]);        /* * a(k) */
            sum += term;
        }
        a_i -= sum;                                 /* subtract the sum */
        a_cache[i] = a_i;
    }
    a_cap = new_cap;
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    ensure_pow_tables(n);
    ensure_a_cache(n);
    return a_cache[n];
}
