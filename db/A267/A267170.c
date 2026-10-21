/*
 * A267170  Growth series for affine Coxeter group B_7.
 * https://oeis.org/A267170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the rational function
 *            ∏_{i=1}^{7} (1 - x^{m_i+1}) / ((1 - x)^7 ∏_{i=1}^{7} (1 -
 *            x^{m_i})), where m_i = 1,3,5,7,9,11,13
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>          /* for memset */

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
/* C11 standard */
#define DEPRECATED_UNUSED __attribute__((unused))
#else
#define DEPRECATED_UNUSED
#endif

/* -------------------------------------------------------------------------- */
/*  CONSTANTS AND TYPE DEFINITIONS                                           */
/* -------------------------------------------------------------------------- */
#define K          7
#define NUM_MS     7
#define MS_VALUES  {1,3,5,7,9,11,13}

#define MAX_DEG_NUM  60            /* sufficient for degree 56 (1 .. 56) */
#define MAX_DEG_DEN  MAX_DEG_NUM   /* denominator and numerator have same max degree */

/* coefficients may be as large as a few hundred; int16_t is sufficient      */
static int16_t  q_coeff[MAX_DEG_DEN + 1];
static int16_t  p_coeff[MAX_DEG_NUM + 1];
static size_t   q_deg;
static size_t   p_deg;
static bool     coeffs_inited = false;

/* -------------------------------------------------------------------------- */
/*  POLYNOMIAL MULTIPLICATION FUNCTION                                        */
/* -------------------------------------------------------------------------- */
static void
multiply_by_1_minus_xm(int16_t *coeff, size_t *deg, size_t m)
{
    /* temporary array for intermediate computation   */
    int32_t tmp[MAX_DEG_DEN + 1] = {0};

    size_t new_deg = *deg + m;
    if (new_deg > MAX_DEG_DEN) return;   /* safety; should not happen */

    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i]        += coeff[i];
        tmp[i + m]    -= coeff[i];
    }
    /* copy back */
    for (size_t i = 0; i <= new_deg; ++i) {
        coeff[i] = (int16_t)tmp[i];
    }
    *deg = new_deg;
}

/* -------------------------------------------------------------------------- */
/*  INITIALIZE COEFFICIENTS OF DENOMINATOR AND NUMERATOR                       */
/* -------------------------------------------------------------------------- */
static void
init_coeffs(void)
{
    if (coeffs_inited) return;

    /* ---------- denominator Q(x) = (1-x)^K * ∏ (1-x^(m_i)) ---------------- */
    q_deg = 0;
    q_coeff[0] = 1;                      /* start with polynomial 1 */

    /* multiply by (1-x) K times (for the external factor) */
    for (size_t t = 0; t < K; ++t) {
        multiply_by_1_minus_xm(q_coeff, &q_deg, 1);
    }

    /* multiply by each (1 - x^{m_i})      */
    const size_t ms[NUM_MS] = MS_VALUES;
    for (size_t i = 0; i < NUM_MS; ++i) {
        multiply_by_1_minus_xm(q_coeff, &q_deg, ms[i]);
    }

    /* ---------- numerator P(x) = ∏ (1 - x^(m_i+1)) ---------------------- */
    p_deg = 0;
    p_coeff[0] = 1;
    for (size_t i = 0; i < NUM_MS; ++i) {
        multiply_by_1_minus_xm(p_coeff, &p_deg, ms[i] + 1);
    }

    coeffs_inited = true;
}

/* -------------------------------------------------------------------------- */
/*  SEQUENCE CACHE                                                             */
/* -------------------------------------------------------------------------- */
static seq_t *a_cache = NULL;   /* dynamic array of computed terms          */
static size_t a_cache_size = 0; /* number of terms already stored           */

static void
ensure_cache_size(size_t n)
{
    if (n < a_cache_size) return;

    size_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    a_cache = new_cache;

    /* compute new terms from a_cache_size up to n */
    for (size_t i = a_cache_size; i <= n; ++i) {
        seq_t add_sum = 0; /* sum for negative q_k coefficients and for p_k ≥ 0 */
        seq_t sub_sum = 0; /* sum for positive q_k coefficients and for p_k < 0 */

        /* handle numerator coefficient p_i */
        int16_t p = p_coeff[i];
        if (p >= 0) {
            add_sum += (seq_t)p;
        } else {
            sub_sum += (seq_t)(-p);
        }

        /* handle denominator coefficients q_k for k >= 1 */
        size_t max_k = (i < q_deg) ? i : q_deg;
        for (size_t k = 1; k <= max_k; ++k) {
            seq_t term = a_cache[i - k] * (seq_t)abs(q_coeff[k]);
            if (q_coeff[k] < 0) {
                add_sum += term;
            } else {
                sub_sum += term;
            }
        }

        /* compute a_i = add_sum - sub_sum   (must be non‑negative) */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        __int128 diff = (__int128)add_sum - (__int128)sub_sum;
        if (diff < 0) diff = 0;   /* safety: recurrence guarantees non‑negativity */
        a_cache[i] = (seq_t)diff;
#else
        /* fall back for older C: use unsigned subtraction assuming no overflow */
        if (add_sum >= sub_sum) {
            a_cache[i] = add_sum - sub_sum;
        } else {
            a_cache[i] = 0; /* should not happen */
        }
#endif
    }

    a_cache_size = new_size;
}

/* -------------------------------------------------------------------------- */
/*  PUBLIC FUNCTION a(idx_t n)                                                */
/* -------------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    init_coeffs();
    ensure_cache_size((size_t)n);
    return a_cache[(size_t)n];
}
