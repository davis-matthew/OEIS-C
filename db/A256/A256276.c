/*
 * A256276  Expansion of q * phi(q) * chi(q^3) * psi(-q^9) in powers of q
 *          where phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A256276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in * q * phi(q) * chi(q³) * psi(‑q⁹)
 * Range:     u8 n <= 25, u16 n <= 789, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product is converted to an eta‑quotient and the coefficients
 * are obtained via the Euler recurrence.  The code uses the
 * auxiliary function mul_small from utils/mul_small.h.   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

static int8_t  *e_arr   = NULL;   /* exponent e(m) for (1‑q^m)  */
static int64_t *c_arr   = NULL;   /* c(k) = –∑_{d|k} d·e(d)     */
static seq_t   *g_arr   = NULL;   /* coefficients of the product */
static size_t   cap     = 0;      /* current allocation size   */

static void ensure_capacity(size_t n)
{
    if (n <= cap)               /* already computed up to n‑1 */
        return;
    if (n == 0)
        return;

    /* Allocate/resize arrays for indices 0 … n‑1 (index 0 unused for e_arr) */
    e_arr = (int8_t *)realloc(e_arr, n * sizeof(int8_t));
    c_arr = (int64_t *)realloc(c_arr, n * sizeof(int64_t));
    g_arr = (seq_t *)realloc(g_arr, n * sizeof(seq_t));

    /* ----------  compute e(m)  ---------- */
    for (size_t m = 1; m < n; ++m) {
        int e = 0;
        if (m % 2  == 0) e += 5;
        if (m % 6  == 0) e += 2;
        if (m % 9  == 0) e += 1;
        if (m % 36 == 0) e += 1;
        /* denominator contributions (always negative) */
        e -= 2;                    /* eta(q)²      */
        if (m % 4  == 0) e -= 2;   /* eta(q⁴)²      */
        if (m % 3  == 0) e -= 1;   /* eta(q³)       */
        if (m % 12 == 0) e -= 1;   /* eta(q¹²)      */
        if (m % 18 == 0) e -= 1;   /* eta(q¹⁸)      */
        e_arr[m] = (int8_t)e;
    }

    /* ----------  compute c(k) = –∑_{d|k} d·e(d)  ---------- */
    for (size_t k = 1; k < n; ++k)
        c_arr[k] = 0;
    for (size_t d = 1; d < n; ++d) {
        int e_d = e_arr[d];
        if (e_d == 0)
            continue;
        int64_t delta = -(int64_t)d * (int64_t)e_d; /* negative sign */
        for (size_t m = d; m < n; m += d)
            c_arr[m] += delta;
    }

    /* ----------  compute product series coefficients g_arr  ---------- */
    g_arr[0] = 1;   /* constant term of the product */
    for (size_t idx = 1; idx < n; ++idx) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        /* sum_{k=1..idx} c(k) * g_arr[idx‑k] */
        for (size_t k = 1; k <= idx; ++k) {
            int64_t ck = c_arr[k];
            if (ck > 0)
                sum_pos += mul_small(g_arr[idx - k], (unsigned int)ck);
            else if (ck < 0)
                sum_neg += mul_small(g_arr[idx - k], (unsigned int)(-ck));
        }
        /* diff = sum_pos – sum_neg  (should be non‑negative) */
        seq_t diff = (sum_pos >= sum_neg) ? (sum_pos - sum_neg)
                                          : (sum_neg - sum_pos);
        g_arr[idx] = diff / (seq_t)idx;
    }

    cap = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset is 1; anything below is undefined */
    ensure_capacity((size_t)n);
    return g_arr[n - 1];            /* a(n) = g_arr[n‑1]  */
}
