/*
 * A257398  Expansion of phi(-x^6)^2 / chi(-x) in powers of x where phi(),
 *          chi() are Ramanujan theta functions.
 * https://oeis.org/A257398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of the period‑12 sequence [1, 0, 1, 0, 1, -4, 1,
 *            0, 1, 0, 1, -2]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; for n >= 1:
   a(n) = (1/n) * Σ_{k=1..n} ( Σ_{d|k} d * b(d) ) * a(n-k) */

/* Includes necessary
   (std::malloc, std::realloc, std::calloc, etc.) */
#include <stdlib.h>
#include <stdint.h>

/* static storage */
static seq_t *a_arr      = NULL;   /* a[n] values   */
static int64_t *d_arr    = NULL;   /* d[n] = Σ_{d|n} d*b(d) */
static size_t cap        = 0;      /* allocated size */

/* period‑12 b(d) array: b(1)=1, b(2)=0, …, b(12)=-2 */
static const int period12[12] = {
    1, 0, 1, 0, 1, -4,
    1, 0, 1, 0, 1, -2
};

/* Main routine */
seq_t a(idx_t n)
{
    /* allocate or extend arrays to hold values up to n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_a = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        if (new_a == NULL) { /* allocation failure, return 0 as safe fallback */
            return (seq_t)0;
        }
        a_arr = new_a;
        int64_t *new_d = (int64_t *)realloc(d_arr, new_cap * sizeof(int64_t));
        if (new_d == NULL) {
            return (seq_t)0;
        }
        d_arr = new_d;
        cap    = new_cap;
    }

    /* initialise a(0) */
    a_arr[0] = (seq_t)1;

    /* --- compute d[1..n] with the divisor‑sum method --- */
    for (idx_t i = 1; i <= n; ++i) d_arr[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int bd = period12[(d - 1) % 12];          /* b(d) */
        int64_t val = (int64_t)d * bd;             /* d * b(d) */
        for (idx_t k = d; k <= n; k += d)
            d_arr[k] += val;
    }

    /* --- compute a[1..n] via the Euler transform recurrence --- */
    for (idx_t i = 1; i <= n; ++i) {
        int64_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += d_arr[k] * (int64_t)a_arr[i - k];
        }
        /* division by i is exact by definition of the transform */
        a_arr[i] = (seq_t)(sum / (int64_t)i);
    }

    return a_arr[n];
}
