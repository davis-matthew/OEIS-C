/*
 * A319234  T(n, k) is the coefficient of x^k of the polynomial p(n) which is
 *          defined as the scalar part of P(n) = Q(x, 1, 1, 1) * P(n-1) for n
 *          > 0 and P(0) = Q(1, 0, 0, 0) where Q(a, b, c, d) is a quaternion,
 *          triangle read by rows.
 * https://oeis.org/A319234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened list of coefficients of the polynomials *
 *            defined by the quaternion recurrence * * p_0(x) = 1, T_0(x) = 0
 *            * p_n(x) = x p_{n-1}(x) - T_{n-1}(x) * T_n(x) = x T_{n-1}(x) + 3
 *            p_{n-1}(x) * * Here p_n(x) is the scalar part of the quaternion
 *            Q(x,1,1,1)^n. * The sequence a(n) lists...
 * Range:     i8 n <= 22, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *terms      = NULL; /* flattened coefficients */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

static seq_t *p_prev  = NULL; /* polynomial p_{k-1} */
static seq_t *T_prev  = NULL; /* polynomial T_{k-1} */
static size_t prev_len = 0;   /* length of p_prev and T_prev */
static size_t cur_deg   = 0;   /* degree of last generated polynomial */

/* Ensure capacity of an array; realloc if necessary */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed + 64;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();     /* out of memory */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Main function to return a(n) */
seq_t a(idx_t n)
{
    /* --- initialization on first call ----------------------------------- */
    if (terms_len == 0) {
        /* start with polynomial p_0 = [1], T_0 = [0] */
        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();

        p_prev = (seq_t *)malloc(sizeof(seq_t));
        T_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!p_prev || !T_prev) abort();

        p_prev[0] = (seq_t)1;
        T_prev[0] = (seq_t)0;
        prev_len   = 1;
        cur_deg    = 0;

        ensure_terms_capacity(1);
        terms[0] = p_prev[0];
        terms_len = 1;
    }

    /* --- generate polynomials until we have index n --------------------- */
    while (terms_len <= (size_t)n) {
        size_t next_len = cur_deg + 2;       /* degree+1 terms */
        seq_t *p_next   = (seq_t *)malloc(next_len * sizeof(seq_t));
        seq_t *T_next   = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!p_next || !T_next) abort();

        for (size_t k = 0; k < next_len; ++k) {
            if (k == 0) {
                p_next[k] = -T_prev[0];
                T_next[k] = (seq_t)3 * p_prev[0];
            } else {
                seq_t p_prev_k1 = (k-1 < prev_len) ? p_prev[k-1] : (seq_t)0;
                seq_t T_prev_k  = (k   < prev_len) ? T_prev[k]   : (seq_t)0;
                seq_t p_prev_k  = (k   < prev_len) ? p_prev[k]   : (seq_t)0;
                seq_t T_prev_k1 = (k-1 < prev_len) ? T_prev[k-1] : (seq_t)0;
                p_next[k] = p_prev_k1 - T_prev_k;
                T_next[k] = T_prev_k1 + (seq_t)3 * p_prev_k;
            }
        }

        /* append new coefficients to the global array */
        ensure_terms_capacity(terms_len + next_len);
        memcpy(terms + terms_len, p_next, next_len * sizeof(seq_t));
        terms_len += next_len;

        /* free old arrays and advance */
        free(p_prev);
        free(T_prev);
        p_prev = p_next;
        T_prev = T_next;
        prev_len = next_len;
        cur_deg++;
    }

    /* the requested term is now available */
    return terms[(size_t)n];
}
