/*
 * A357026  E.g.f. satisfies A(x) = (1 - x)^(log(1 - x) * A(x)).
 * https://oeis.org/A357026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2*k)! * (k+1)^(k-1) *
 *            |Stirling1(n,2*k)| / k! a(0) = 1 (Stirling1 = unsigned Stirling
 *            numbers of the first kind) Implements the E.g.f. A(x) = exp(
 *            -LambertW(-log(1-x)^2) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* --- global caches ----------------------------------------------------- */

static seq_t *a_cache   = NULL;   /* a(n) values                  */
static size_t a_cap     = 0;      /* capacity of a_cache          */

static seq_t *s_prev    = NULL;   /* Stirling row for n-1         */
static seq_t *s_curr    = NULL;   /* Stirling row for n           */
static size_t s_cap_a   = 0;      /* capacity of s_prev           */
static size_t s_cap_b   = 0;      /* capacity of s_curr           */

static seq_t *prefactor = NULL;   /* (2k)!/k! * (k+1)^(k-1)       */
static size_t pref_cap  = 0;      /* capacity of prefactor array  */
static seq_t pref_last_ratio = 1; /* (2k)!/k! for the last computed k */

static idx_t cur_n = 0;            /* largest n already computed   */

/* --------------------------------------------------------------------- */
/* helpers ---------------------------------------------------------------- */

static void ensure_capacity(size_t needed, size_t *cap, void **ptr)
{
    if (needed <= *cap) {
        if (*ptr == NULL) {
            *ptr = (void*)malloc(needed * sizeof(seq_t));
            *cap = needed;
        }
        return;
    }
    size_t new_cap = (*cap) ? (*cap) * 2 : 1;
    while (new_cap < needed) new_cap <<= 1;
    *ptr = (void*)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

static void compute_prefactor_upto(idx_t kmax)
{
    size_t need = (size_t)kmax + 1;
    if (need <= pref_cap) return;

    /* enlarge array */
    size_t new_cap = pref_cap ? pref_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    prefactor = (seq_t*)realloc(prefactor, new_cap * sizeof(seq_t));

    size_t start = pref_cap;
    if (pref_cap == 0) {
        prefactor[0] = 1;          /* (0)!/0! * 1^(-1) = 1 */
        pref_last_ratio = 1;       /* (0)!/0!           */
        start = 1;
    }

    for (size_t i = start; i < new_cap; ++i) {
        /* compute (2i)!/i! using the previous ratio */
        seq_t ratio = pref_last_ratio;
        ratio = ratio * (seq_t)(2 * i - 1);
        ratio = ratio * (seq_t)(2 * i);
        ratio = ratio / (seq_t)i;
        pref_last_ratio = ratio;

        /* compute (i+1)^(i-1) iteratively */
        seq_t pow_val = 1;
        for (size_t e = 1; e < i; ++e)
            pow_val = pow_val * (seq_t)(i + 1);

        prefactor[i] = ratio * pow_val;
    }
    pref_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* build sequence up to n ------------------------------------------------ */

static void ensure_computed(idx_t n)
{
    if (n <= cur_n) return;

    /* guarantee space for current n */
    ensure_capacity((size_t)(n + 1), &a_cap,   (void **)&a_cache);
    ensure_capacity((size_t)(n + 1), &s_cap_a, (void **)&s_prev);
    ensure_capacity((size_t)(n + 1), &s_cap_b, (void **)&s_curr);
    compute_prefactor_upto(n / 2);

    if (cur_n == 0) {
        s_prev[0] = (seq_t)1;     /* s(0,0) = 1 */
        s_curr[0] = 0;            /* dummy for safety */
    }

    for (idx_t m = cur_n + 1; m <= n; ++m) {
        s_curr[0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = s_prev[k - 1];
            seq_t mul  = 0;
            if (k <= cur_n) mul = (seq_t)(m - 1) * s_prev[k];
            s_curr[k] = term + mul;
        }

        /* compute a(m) */
        seq_t sum = 0;
        idx_t maxk = m / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t val = prefactor[k] * s_curr[2 * k];
            sum += val;
        }
        a_cache[(size_t)m] = sum;

        /* swap rows */
        seq_t *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
        cur_n = m;
    }
}

/* --------------------------------------------------------------------- */
/* entry point ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* offset 0 */
    ensure_computed(n);
    return a_cache[(size_t)n];
}
