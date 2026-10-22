/*
 * A327758  Expansion of Product_{k>=1} 1/(1 - x^k)^(3/k), where (m/n) is the
 *          Kronecker symbol.
 * https://oeis.org/A327758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 134, i32 n <= 409, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A327758: a(n) = coefficient of ∑_{n≥0} a(n) x^n in  
 *    ∏_{k≥1} 1/(1−x^k)^{(3/k)}
 *   where (3/k) is the Kronecker symbol.
 *
 * Recurrence (derived from logarithmic derivatives):
 *   Let s_k   = (3/k)      (−1, 0 or +1)
 *       f_k   = ∑_{d|k} d · s_d
 * Then a(0) = 1 and for n ≥ 1
 *   a(n) = (1/n) · Σ_{k=1}^{n} f_k · a(n−k)
 *
 * The function below implements this recurrence with pre‑computation
 * of the s_k, f_k and a(n) arrays.  It is memoised across calls.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"   /* provides void build_spf(idx_t limit, idx_t **spf_out); */

static idx_t   max_prepared = 0;   /* largest n for which the arrays are ready */
static seq_t  *a_arr  = NULL;      /* a[0..max_prepared]          */
static seq_t  *f_arr  = NULL;      /* f[0..max_prepared]          */
static int8_t *s_arr  = NULL;      /* s[0..max_prepared]          */
static idx_t  *spf_arr = NULL;     /* smallest prime factor array */

/* --- Helper to compute s_k = (3/k) for 1 <= k <= n --------------------- */
static void compute_s_array(idx_t n)
{
    if (s_arr) free(s_arr);
    s_arr = (int8_t*)calloc((size_t)n + 1, sizeof(int8_t));
    if (!s_arr) exit(1);

    s_arr[1] = 1;                    /* (3/1) = 1 */
    for (idx_t i = 2; i <= n; ++i) {
        if (i % 3 == 0) {            /* any factor 3 → symbol 0 */
            s_arr[i] = 0;
            continue;
        }
        int val = 1;
        idx_t t = i;
        while (t > 1) {
            idx_t p = spf_arr[t];
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            int sym = (p == 3) ? 0
                               : ((p % 12 == 1 || p % 12 == 11) ? 1 : -1);
            if (sym == 0) { val = 0; break; }
            if (e & 1) val *= sym;
        }
        s_arr[i] = (int8_t)val;
    }
}

/* --- Helper to compute f_k = Σ_{d|k} d·s_d for 1 <= k <= n -------------- */
static void compute_f_array(idx_t n)
{
    if (f_arr) free(f_arr);
    f_arr = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!f_arr) exit(1);

    for (idx_t k = 1; k <= n; ++k) {
        seq_t valk = (seq_t)k * (seq_t)s_arr[k];
        for (idx_t m = k; m <= n; m += k) {
            f_arr[m] += valk;
        }
    }
}

/* --- Helper to compute a[n] via the recurrence ----------------------- */
static void compute_a_array(idx_t max_n)
{
    if (a_arr) free(a_arr);
    a_arr = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t));
    if (!a_arr) exit(1);

    a_arr[0] = (seq_t)1;
    for (idx_t n = 1; n <= max_n; ++n) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k) {
            sum += f_arr[k] * a_arr[n - k];
        }
        a_arr[n] = sum / (seq_t)n;      /* exact division */
    }
}

/* --- Prepare all helper arrays up to n -------------------------------- */
static void prepare_upto(idx_t n)
{
    if (n <= max_prepared) return;

    /* Free and rebuild SPF array */
    if (spf_arr) { free(spf_arr); spf_arr = NULL; }
    build_spf(n, &spf_arr);

    compute_s_array(n);
    compute_f_array(n);
    compute_a_array(n);

    max_prepared = n;
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */
    prepare_upto(n);
    return a_arr[n];
}
