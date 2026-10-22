/*
 * A363237  Number of partitions of n with rank a multiple of 5.
 * https://oeis.org/A363237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363237: Number of partitions of n whose rank is a multiple of 5.
   Generating function:
   G(x) = (1 / Π_{k≥1} (1 - x^k)) *
          Σ_{k≥1} (-1)^{k-1} x^{k(3k-1)/2} (1 - x^k)(1 + x^{5k})/(1 - x^{5k})   */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Caching structures */
static seq_t *p_arr   = NULL; /* partition numbers p[0..n] */
static long long *s_arr = NULL; /* coefficients of the numerator series  */
static seq_t *a_pos = NULL; /* accumulating positive contributions   */
static seq_t *a_neg = NULL; /* accumulating negative contributions   */
static idx_t cached_n = 0;   /* maximum n that has been pre‑computed */

/* zero out a seq_t array (used in several places) */
static void zero_seq_array(seq_t *arr, idx_t size)
{
    seq_t zero = 0;
    for (idx_t i = 0; i < size; ++i)
        arr[i] = zero;
}

/* compute ordinary partition numbers p[0..limit] */
static void compute_partitions(idx_t limit)
{
    p_arr = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    zero_seq_array(p_arr, limit + 1);
    p_arr[0] = (seq_t)1;
    for (idx_t k = 1; k <= limit; ++k) {
        for (idx_t i = k; i <= limit; ++i)
            p_arr[i] += p_arr[i - k];
    }
}

/* build the coefficient array s[0..limit] for the numerator series */
static void compute_s_series(idx_t limit)
{
    s_arr = (long long *)malloc((limit + 1) * sizeof(long long));
    memset(s_arr, 0, (limit + 1) * sizeof(long long));
    for (idx_t k = 1; ; ++k) {
        idx_t Gk = (k * (3 * k - 1)) / 2;
        if (Gk > limit) break;
        long long sign = (k & 1) ? 1 : -1;          /* (-1)^(k-1) */
        /* base term    x^{Gk} */
        s_arr[Gk] += sign;
        /* subtract x^{Gk+k} */
        if (Gk + k <= limit)
            s_arr[Gk + k] -= sign;
        /* plus 2 * Σ_{m≥1} x^{Gk + m*5k}  */
        /* minus 2 * Σ_{m≥1} x^{Gk + m*5k + k} */
        idx_t shift = 5 * k;
        while (Gk + shift <= limit) {
            s_arr[Gk + shift] += 2 * sign;
            if (Gk + shift + k <= limit)
                s_arr[Gk + shift + k] -= 2 * sign;
            shift += 5 * k;
        }
    }
}

/* convolve partition numbers with the numerator coefficient array */
static void compute_convolution(idx_t limit)
{
    a_pos = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    a_neg = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    zero_seq_array(a_pos, limit + 1);
    zero_seq_array(a_neg, limit + 1);

    for (idx_t i = 0; i <= limit; ++i) {
        long long coeff = s_arr[i];
        if (!coeff) continue;
        bool pos = coeff > 0;
        seq_t mult = (seq_t)(pos ? coeff : -coeff);  /* |coeff| as seq_t */

        for (idx_t idx = i; idx <= limit; ++idx) {
            seq_t term = p_arr[idx - i] * mult;   /* product of unsigned values */
            if (pos)
                a_pos[idx] += term;
            else
                a_neg[idx] += term;
        }
    }
}

/* ensure all arrays are built up to at least n */
static void ensure_cache(idx_t n)
{
    if (cached_n >= n) return;      /* already ready */

    /* free old memory */
    free(p_arr);
    free(s_arr);
    free(a_pos);
    free(a_neg);

    compute_partitions(n);
    compute_s_series(n);
    compute_convolution(n);
    cached_n = n;
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside OEIS offset */
    ensure_cache(n);
    seq_t pos = a_pos[n];
    seq_t neg = a_neg[n];
    return (pos >= neg) ? (pos - neg) : (neg - pos);
}
