/*
 * A370908  Expansion of e.g.f. (1/x) * Series_Reversion( 4*x/(3 + exp(4*x))
 *          ).
 * https://oeis.org/A370908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} 4^{n-k} * S(n,k) / (n-k+1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* S(n,k) are Stirling numbers of the second kind, computed in place. */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic array helpers --------------------------------------------- */
static void resize_array(size_t *cap, seq_t **ptr, size_t needed)
{
    if (*cap < needed) {
        size_t old = *cap;
        seq_t *tmp = (seq_t *)realloc(*ptr, needed * sizeof(seq_t));
        if (!tmp) abort();
        *ptr = tmp;
        *cap = needed;
        /* zero the new part if array is S (needed for correct Stirling update) */
        for (size_t i = old; i < needed; ++i)
            (*ptr)[i] = 0;
    }
}

/* static storage ----------------------------------------------- */
static idx_t        computed_max = 0;   /* highest n for which a(n) is known */
static seq_t *      a_cache = NULL;    /* a(0..computed_max) */
static size_t       a_cap   = 0;
static seq_t *      fac = NULL;        /* factorials fac[i] = i! */
static size_t       fac_cap = 0;
static seq_t *      pow4 = NULL;       /* powers of 4: pow4[i] = 4^i */
static size_t       pow4_cap = 0;
static seq_t *      S   = NULL;        /* current Stirling row */
static size_t       S_cap  = 0;

/* ------------------------------------------------------------------ */
static void ensure_initialisation(void)
{
    if (a_cache) return; /* already initialised */

    /* a(0) */
    resize_array(&a_cap, &a_cache, 1);
    a_cache[0] = (seq_t)1;
    computed_max = 0;

    /* factorials */
    resize_array(&fac_cap, &fac, 1);
    fac[0] = (seq_t)1;

    /* powers of 4 */
    resize_array(&pow4_cap, &pow4, 1);
    pow4[0] = (seq_t)1;

    /* Stirling row for n = 0 : S(0,0)=1 */
    resize_array(&S_cap, &S, 1);
    S[0] = (seq_t)1;
}

/* compute all values up to n */
static void compute_up_to(idx_t n)
{
    if (n <= computed_max) return;
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        /* extend factorial */
        resize_array(&fac_cap, &fac, m + 1);
        fac[m] = fac[m - 1] * (seq_t)m;

        /* extend powers of 4 */
        resize_array(&pow4_cap, &pow4, m + 1);
        pow4[m] = pow4[m - 1] * (seq_t)4;

        /* extend Stirling array */
        resize_array(&S_cap, &S, m + 1);

        /* update Stirling row: S_new[k] = k*S_old[k] + S_old[k-1] */
        for (idx_t k = m; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = 0;             /* S(m,0)=0 for m>0 */

        /* compute a(m) */
        seq_t sum   = 0;
        seq_t ratio = fac[m];              /* = fac[m]/fac[1] */
        for (idx_t k = m; k >= 1; --k) {
            seq_t term = S[k];
            term *= pow4[m - k];
            term *= ratio;
            sum += term;
            if (k > 1) {
                /* ratio_{k-1} = ratio / (m - k + 2) */
                ratio /= (seq_t)(m - k + 2);
            }
        }

        /* store result */
        resize_array(&a_cap, &a_cache, m + 1);
        a_cache[m] = sum;
    }
    computed_max = n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_initialisation();
    compute_up_to(n);
    return a_cache[n];
}
