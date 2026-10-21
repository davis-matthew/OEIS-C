/*
 * A276232  E.g.f. W(x)^W(x), where W(x) = LambertW(-x)/(-x) and satisfies
 *          W(x)^(1/W(x)) = exp(x).
 * https://oeis.org/A276232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n / n! in exp(LambertW(-x)^2/x) /*
 *            a(0)=1 and for n>=1, a(n)=∑_{k=1..n} C(n-1,k-1)*f(k)*a(n-k) *
 *            where f(k)=∑_{i=1..k} C(k,i)*i^{i-1}*(k+1-i)^{k-i}/(k+1-i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   bitlen_seq_t, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t base, int exp) */
#include "../utils/bitlen_seq_t.h"

static seq_t *cache_a = NULL;   /* a[0..max_comp] */
static size_t cap_a = 0;
static size_t last_a = 0;       /* last index computed in a */

static seq_t *cache_f = NULL;   /* f[1..max_comp] (f[0] unused) */
static size_t cap_f = 0;
static size_t last_f = 0;       /* last index computed in f */

static seq_t *cache_t = NULL;   /* t[i] = i^{i-1}, t[0] unused */
static size_t cap_t = 0;
static size_t last_t = 0;       /* last index computed in t */

/* enlarge array for a */
static void ensure_cap_a(size_t n)
{
    if (n < cap_a) return;
    size_t new_cap = cap_a ? cap_a * 2 : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
    cache_a = tmp;
    cap_a = new_cap;
}

/* enlarge array for f and t simultaneously */
static void ensure_cap_f_t(size_t n)
{
    if (n < cap_f) return;
    size_t new_cap = cap_f ? cap_f * 2 : 64;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp_f = (seq_t *)realloc(cache_f, new_cap * sizeof(seq_t));
    seq_t *tmp_t = (seq_t *)realloc(cache_t, new_cap * sizeof(seq_t));
    cache_f = tmp_f;
    cache_t = tmp_t;
    cap_f = cap_t = new_cap;
}

/* precompute t[i] = i^{i-1} for i up to n */
static void compute_t_up_to(size_t n)
{
    if (n <= last_t) return;
    ensure_cap_f_t(n);
    for (size_t i = last_t + 1; i <= n; ++i) {
        if (i == 0) {
            cache_t[i] = (seq_t)1;
        } else {
            cache_t[i] = pow_seq((seq_t)i, (int)(i - 1));
        }
    }
    last_t = n;
}

/* precompute f[k] for k up to n */
static void compute_f_up_to(size_t n)
{
    if (n <= last_f) return;
    ensure_cap_f_t(n);
    /* ensure t array */
    compute_t_up_to(n);
    for (size_t k = last_f + 1; k <= n; ++k) {
        seq_t sum = 0;
        seq_t bin = (seq_t)k;  /* C(k,1) */
        for (size_t i = 1; i <= k; ++i) {
            /* term = bin * t[i] * (k+1-i)^{k-i} / (k+1-i) */
            seq_t pow_i = pow_seq((seq_t)(k + 1 - i), (int)(k - i));
            seq_t term = bin * cache_t[i];
            term *= pow_i;
            term /= (seq_t)(k + 1 - i);
            sum += term;

            /* update bin for next i */
            if (i < k) {
                bin = bin * (seq_t)(k - i) / (seq_t)(i + 1);
            }
        }
        cache_f[k] = sum;
    }
    last_f = n;
}

/* ensure a[0..n] computed */
static void compute_a_up_to(size_t n)
{
    if (n <= last_a) return;
    ensure_cap_a(n);
    if (cache_a[0] == 0) {  /* first time for a[0] */
        cache_a[0] = (seq_t)1;
        last_a = 0;
    }
    /* compute missing a values */
    for (size_t nn = last_a + 1; nn <= n; ++nn) {
        /* we need f up to nn for recurrence */
        compute_f_up_to(nn);
        seq_t sum = 0;
        seq_t bin = 1;   /* C(nn-1,0) */
        for (size_t k = 1; k <= nn; ++k) {
            seq_t term = bin * cache_f[k];
            term *= cache_a[nn - k];
            sum += term;
            if (k < nn) {
                bin = bin * (seq_t)(nn - k) / (seq_t)k;
            }
        }
        cache_a[nn] = sum;
    }
    last_a = n;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;  /* guard for very large n that may overflow idx_t when cast to size_t */
    compute_a_up_to((size_t)n);
    return cache_a[(size_t)n];
}
