/*
 * A329276  Expansion of 1 / (1 - Sum_{k>=1} mu(2*k) * log(1 - 2 * x^k) / (2 *
 *          k)), where mu = A008683.
 * https://oeis.org/A329276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A000048(k) * a(n-k), a(0)=1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   compute_mu_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000048(k) is computed from the general definition
   t(k) = -(1/(2k)) * sum_{d|k} mu(2d) * 2^{k/d} .
   The sequence a is the convolution inverse of this t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_mu_arr.h"

static signed char *mu_arr = NULL;
static idx_t mu_limit = 0;

static void ensure_mu(idx_t limit)
{
    if (limit <= mu_limit) return;
    signed char *new_mu = compute_mu_arr(limit);
    free(mu_arr);
    mu_arr   = new_mu;
    mu_limit = limit;
}

static seq_t *pow2_arr = NULL;
static size_t pow2_len = 0;          /* number of entries */

static void ensure_pow2(idx_t limit)
{
    if (pow2_len == 0) {
        pow2_arr = (seq_t*)malloc(sizeof(seq_t));
        pow2_arr[0] = (seq_t)1;
        pow2_len   = 1;
    }
    if (pow2_len > limit) return;
    size_t new_len = (size_t)limit + 1;
    pow2_arr = (seq_t*)realloc(pow2_arr, new_len * sizeof(seq_t));
    for (idx_t i = pow2_len; i <= limit; ++i) {
        pow2_arr[i] = pow2_arr[i - 1] * (seq_t)2;
    }
    pow2_len = new_len;
}

static seq_t *t_arr = NULL;
static size_t t_len = 0;            /* number of entries */

static void ensure_t(idx_t limit)
{
    if (t_len > limit) return;
    if (t_len == 0) {               /* allocate t[0] for convenience */
        t_arr = (seq_t*)malloc(sizeof(seq_t));
        t_arr[0] = (seq_t)0;
        t_len = 1;
    }
    size_t new_len = (size_t)limit + 1;
    t_arr = (seq_t*)realloc(t_arr, new_len * sizeof(seq_t));

    ensure_pow2(limit);
    ensure_mu(limit * 2);            /* we need mu(2*d) for d up to limit */

    for (idx_t k = t_len; k <= limit; ++k) {
        seq_t pos = 0, sub = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                signed char mu_val = mu_arr[2 * d];
                idx_t exp = k / d;
                seq_t val = pow2_arr[exp];
                if (mu_val > 0) pos += val;
                else if (mu_val < 0) sub += val;

                idx_t d2 = k / d;
                if (d2 != d) {
                    mu_val = mu_arr[2 * d2];
                    exp = d;                 /* k / d2 == d */
                    val = pow2_arr[exp];
                    if (mu_val > 0) pos += val;
                    else if (mu_val < 0) sub += val;
                }
            }
        }
        seq_t diff = sub;
        diff -= pos;                  /* sub > pos for all k >= 1 */
        seq_t denom = ((seq_t)2) * ((seq_t)k);
        t_arr[k] = diff / denom;
    }
    t_len = new_len;
}

static seq_t *a_arr = NULL;
static size_t a_len = 0;            /* number of entries */

static void ensure_a(idx_t limit)
{
    if (a_len > limit) return;
    if (a_len == 0) {               /* allocate a[0] and initialize */
        a_arr = (seq_t*)malloc(sizeof(seq_t));
        a_arr[0] = (seq_t)1;
        a_len = 1;
    }
    size_t new_len = (size_t)limit + 1;
    a_arr = (seq_t*)realloc(a_arr, new_len * sizeof(seq_t));

    ensure_t(limit);

    for (idx_t k = a_len; k <= limit; ++k) {
        if (k == 0) { a_arr[0] = (seq_t)1; continue; }
        seq_t sum = 0;
        for (idx_t i = 1; i <= k; ++i) {
            sum += t_arr[i] * a_arr[k - i];
        }
        a_arr[k] = sum;
    }
    a_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 */
    ensure_a(n);
    return a_arr[n];
}
