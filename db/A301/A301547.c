/*
 * A301547  Expansion of Product_{k>=1} 1/(1 - x^k)^(sigma_9(k)).
 * https://oeis.org/A301547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301547: Expansion of Product_{k>=1} 1/(1 - x^k)^{σ₉(k)}
   Exact computation of a(n) via the recurrence
      a(0) = 1
      a(n) = (1/n) * Σ_{k=1}^n B_k * a(n-k)         (n ≥ 1)
   where B_k = Σ_{d|k} d * σ₉(d).
   The implementation is fully generic: it works for
   unsigned 8‑64‑bit integers and for arbitrary‑precision
   integer types chosen as seq_t.  */
#include <stdint.h>
#ifndef __cplusplus
#include <stdlib.h>
#endif

/* ---------- C implementation ---------- */
#ifndef __cplusplus
/* local scratch multiplication for small exponents */
static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

static idx_t cap = 0;
static seq_t *a_arr = NULL, *sigma10_arr = NULL, *r_arr = NULL;

static void recompute_tables(idx_t limit)
{
    /* free previous allocations */
    if (a_arr)        free(a_arr);
    if (sigma10_arr)  free(sigma10_arr);
    if (r_arr)        free(r_arr);

    /* allocate new arrays */
    a_arr        = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    sigma10_arr  = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    r_arr        = (seq_t *)malloc((limit + 1) * sizeof(seq_t));

    /* initialise to zero */
    for (idx_t i = 0; i <= limit; ++i) {
        a_arr[i]        = (seq_t)0;
        sigma10_arr[i]  = (seq_t)0;
        r_arr[i]        = (seq_t)0;
    }

    /* σ₁₀(n) = Σ_{t|n} t¹⁰ */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t pk10 = pow_small((seq_t)k, 10);
        for (idx_t m = k; m <= limit; m += k)
            sigma10_arr[m] += pk10;
    }

    /* B_m = Σ_{d|m} d * σ₉(d)  (recurrence coefficient) */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t val = sigma10_arr[d]; /* σ₁₀(d) = Σ_{t|d} t¹⁰ */
        for (idx_t m = d; m <= limit; m += d) {
            seq_t mult = (seq_t)(m / d);
            r_arr[m] += mult * val;
        }
    }

    /* a(0)=1; compute subsequent values */
    a_arr[0] = (seq_t)1;
    for (idx_t n = 1; n <= limit; ++n) {
        unsigned __int128 sum = 0;
        for (idx_t k = 1; k <= n; ++k)
            sum += (unsigned __int128)r_arr[k] *
                   (unsigned __int128)a_arr[n - k];
        a_arr[n] = (seq_t)(sum / (unsigned __int128)n);
    }

    cap = limit + 1;
}

seq_t a(idx_t n)
{
    if (n >= cap)
        recompute_tables(n);
    return a_arr[n];
}

/* ---------- C++ implementation ---------- */
#else
#include <vector>
#include <cstdint>

using std::vector;

static idx_t cap = 0;
static vector<seq_t> a_arr, sigma10_arr, r_arr;

static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

static void recompute_tables(idx_t limit)
{
    a_arr.assign(limit + 1, 0);
    sigma10_arr.assign(limit + 1, 0);
    r_arr.assign(limit + 1, 0);

    /* σ₁₀(n) */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t pk10 = pow_small((seq_t)k, 10);
        for (idx_t m = k; m <= limit; m += k)
            sigma10_arr[m] += pk10;
    }

    /* B_m */
    for (idx_t d = 1; d <= limit; ++d) {
        seq_t val = sigma10_arr[d];
        for (idx_t m = d; m <= limit; m += d) {
            seq_t mult = (seq_t)(m / d);
            r_arr[m] += mult * val;
        }
    }

    /* recurrence */
    a_arr[0] = 1;
    for (idx_t n = 1; n <= limit; ++n) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= n; ++k)
            sum += r_arr[k] * a_arr[n - k];
        a_arr[n] = sum / n;
    }

    cap = limit + 1;
}

seq_t a(idx_t n)
{
    if (n >= cap)
        recompute_tables(n);
    return a_arr[n];
}
#endif
