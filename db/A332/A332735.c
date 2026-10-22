/*
 * A332735  Numbers of graphs which are double triangle descendants of K_5
 *          with four more vertices than triangles.
 * https://oeis.org/A332735
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the coefficient of x^n in 
 *   x^9*(1+4x+3x^2+6x^3+3x^4+4x^5+4x^7-3x^8+3x^9-x^10+x^11)
 *   divided by (1-x)^4*(1+x)^2*(1+x^2).
 * The coefficients of S(x)=1/[(1-x)^4(1+x)^2(1+x^2)] satisfy the linear
 * recurrence:
 *   s_n = 2*s_{n-1} - 2*s_{n-3} + 2*s_{n-4} - 2*s_{n-5} + 2*s_{n-7} - s_{n-8},
 *   with s_0=1 and s_k=0 for k<0. Then a_n = Σ_{k∈K} c_k * s_{n-9-k},
 *   where K={0,1,2,3,4,5,7,8,9,10,11} and the coefficients c_k are
 *   {1,4,3,6,3,4,4,-3,3,-1,1}.
 */

#include <stdlib.h>
#include <stddef.h>

/* Global caches for the series s(x)=1/D(x) and the sequence a(n) */
static seq_t *s_arr   = NULL;   /* coefficients s_n of 1/D(x)      */
static size_t s_cap   = 0;      /* capacity of s_arr               */
static idx_t  s_len   = 0;      /* number of s values computed     */

static seq_t *a_arr   = NULL;   /* a(n) values                     */
static size_t a_cap   = 0;      /* capacity of a_arr               */
static idx_t  a_len   = 0;      /* number of a values computed     */

/* Helper arrays for the convolution (k, c_k) pairs */
static const int P_K[11] = {0,1,2,3,4,5,7,8,9,10,11};
static const int P_C[11] = {1,4,3,6,3,4,4,-3,3,-1,1};

/* Ensure that s_arr contains values up to index n (inclusive) */
static void ensure_s_up_to(idx_t n)
{
    if (n < s_len) return;            /* already computed */

    size_t needed = (size_t)n + 1;
    if (needed > s_cap) {
        s_cap = needed;
        s_arr = (seq_t *)realloc(s_arr, s_cap * sizeof(seq_t));
        if (!s_arr) return;           /* abort on allocation failure */
    }

    /* Initialize s_0 if not yet done */
    if (s_len == 0) {
        s_arr[0] = (seq_t)1;
        s_len   = 1;
    }

    for (idx_t i = s_len; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t pos = 0, neg = 0;

        if (i >= 1) pos += (seq_t)2 * s_arr[idx-1];
        if (i >= 4) pos += (seq_t)2 * s_arr[idx-4];
        if (i >= 7) pos += (seq_t)2 * s_arr[idx-7];

        if (i >= 3) neg += (seq_t)2 * s_arr[idx-3];
        if (i >= 5) neg += (seq_t)2 * s_arr[idx-5];
        if (i >= 8) neg += s_arr[idx-8];   /* coefficient -1 */

        s_arr[idx] = pos > neg ? pos - neg : (seq_t)0;
    }

    s_len = n + 1;
}

/* Ensure that a_arr contains values up to index n (inclusive) */
static void ensure_a_up_to(idx_t n)
{
    if (n < a_len) return;            /* already computed */

    /* a(n) depends on s up to index n (since the largest k=11 gives s_{n-20}) */
    ensure_s_up_to(n);

    size_t needed = (size_t)n + 1;
    if (needed > a_cap) {
        a_cap = needed;
        a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        if (!a_arr) return;           /* abort on allocation failure */
    }

    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum_pos = 0, sum_neg = 0;

        if (i >= 9) {                  /* values below offset are irrelevant */
            for (int j = 0; j < 11; ++j) {
                const int k = P_K[j];
                const int c = P_C[j];
                if (c == 0) continue;

                if (i < 9 + (idx_t)k) continue;  /* negative index */
                idx_t idx = i - 9 - (idx_t)k;

                if (c > 0)
                    sum_pos += (seq_t)c * s_arr[(size_t)idx];
                else
                    sum_neg += (seq_t)(-c) * s_arr[(size_t)idx];
            }
        }

        a_arr[(size_t)i] = sum_pos > sum_neg ? sum_pos - sum_neg : (seq_t)0;
    }

    a_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;    /* below offset – value not specified */
    ensure_a_up_to(n);
    return a_arr[(size_t)n];
}
