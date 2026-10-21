/*
 * A292617  Convolution of number of partitions into distinct parts and
 *          Catalan numbers.
 * https://oeis.org/A292617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} p_distinct(k) * C_{n-k} where p_distinct(k) =
 *            number of partitions of k into distinct parts and C_m = m‑th
 *            Catalan number
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   idx_to_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/idx_to_size.h"

/* -------------  internal helpers  ------------------------------------- */

/* -------------- state for memoisation --------------------------------- */

static bool   _initialized = false;   /* has the tables been computed?   */
static idx_t  _max_n       = 0;        /* highest index that has been built */
static seq_t * _q          = NULL;    /* partitions into distinct parts   */
static seq_t * _c          = NULL;    /* Catalan numbers                 */
static seq_t * _a          = NULL;    /* convolution result              */

/*  recompute tables up to N (inclusive) -------------------------------- */
static void _compute_up_to(idx_t N)
{
    size_t sz = idx_to_size(N) + 1;

    /* grow / allocate working arrays */
    _q = (seq_t *)realloc(_q, sz * sizeof(seq_t));
    _c = (seq_t *)realloc(_c, sz * sizeof(seq_t));
    _a = (seq_t *)realloc(_a, sz * sizeof(seq_t));

    /* --------- partitions into distinct parts (DP) --------- */
    for (size_t i = 0; i < sz; ++i) _q[i] = 0;
    _q[0] = (seq_t)1;
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t i = N; i >= part; --i) {
            _q[idx_to_size(i)] += _q[idx_to_size(i - part)];
        }
    }

    /* --------- Catalan numbers (recursive formula) -------- */
    _c[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        /* C(i) = C(i-1) * 2*(2*(i-1)+1) / (i+1)  – exact division */
        seq_t num = _c[idx_to_size(i - 1)];
        num = num * ((seq_t)2 * ((seq_t)2 * (i - 1) + (seq_t)1));
        seq_t den = (seq_t)(i + 1);
        _c[idx_to_size(i)] = num / den;
    }

    /* --------- convolution a(n) = Σ p(k)·C(n-k) --------- */
    for (size_t n = 0; n < sz; ++n) {
        seq_t sum = 0;
        for (size_t k = 0; k <= n; ++k) {
            sum += _q[k] * _c[n - k];
        }
        _a[n] = sum;
    }

    _max_n = N;
    _initialized = true;
}

/* --------------- public entry point  --------------------------------- */
seq_t a(idx_t n)
{
    if (! _initialized || n > _max_n) {
        _compute_up_to(n);
    }
    return _a[idx_to_size(n)];
}
