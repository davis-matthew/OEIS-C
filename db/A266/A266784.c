/*
 * A266784  The growth series for the affine Weyl group F_4.
 * https://oeis.org/A266784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266784 – growth series for the affine Weyl group F4.
   Recurrence derived from the rational generating function:
   G(t) = (1-x²)(1-x⁶)(1-x⁸)(1-x¹²) /
          [(1-x)⁵(1-x⁵)(1-x⁷)(1-x¹¹)].
*/

#include <stddef.h>
#ifdef __cplusplus
#  include <new>
#endif

/* ------------------------------------------------------------------ */
/*  Coefficient tables for the denominator D(t) and numerator N(t)     */
/* ------------------------------------------------------------------ */
static int D_coef[29];   /* indices 0..28, D[0] == 1 */
static int N_coef[29];   /* indices 0..28, N[0] == 1 */

/* build the coefficient tables (executed once) */
static void init_coeffs(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    /* Helper temporary buffer */
    int tmp[29];
    int cur_deg, i, k;

    /* ----------  D(t) = (1-x)^5 (1-x^5)(1-x^7)(1-x^11)  ---------- */
    int poly[29] = {0};
    cur_deg = 0;
    poly[0] = 1; /* start with 1 */

    /* multiply by (1-x) five times */
    for (k = 0; k < 5; ++k) {
        for (i = 0; i <= cur_deg + 1; ++i) tmp[i] = 0;
        for (i = 0; i <= cur_deg; ++i) {
            tmp[i]     += poly[i];
            tmp[i + 1] -= poly[i];
        }
        for (i = 0; i <= cur_deg + 1; ++i) poly[i] = tmp[i];
        ++cur_deg;          /* degree increased by 1 */
    }

    /* multiply by (1-x^5) */
    for (i = 0; i <= cur_deg + 5; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += poly[i];
        tmp[i + 5] -= poly[i];
    }
    cur_deg += 5;
    for (i = 0; i <= cur_deg; ++i) poly[i] = tmp[i];

    /* multiply by (1-x^7) */
    for (i = 0; i <= cur_deg + 7; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += poly[i];
        tmp[i + 7] -= poly[i];
    }
    cur_deg += 7;
    for (i = 0; i <= cur_deg; ++i) poly[i] = tmp[i];

    /* multiply by (1-x^11) */
    for (i = 0; i <= cur_deg + 11; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += poly[i];
        tmp[i + 11] -= poly[i];
    }
    cur_deg += 11;
    for (i = 0; i <= cur_deg; ++i) poly[i] = tmp[i];

    /* copy coefficients to D_coef */
    for (i = 0; i <= 28; ++i) D_coef[i] = poly[i];

    /* ----------  N(t) = (1-x²)(1-x⁶)(1-x⁸)(1-x¹²)  ---------- */
    int polyN[29] = {0};
    cur_deg = 0;
    polyN[0] = 1; /* start with 1 */

    /* multiply by (1-x²) */
    for (i = 0; i <= cur_deg + 2; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += polyN[i];
        tmp[i + 2] -= polyN[i];
    }
    cur_deg += 2;
    for (i = 0; i <= cur_deg; ++i) polyN[i] = tmp[i];

    /* multiply by (1-x⁶) */
    for (i = 0; i <= cur_deg + 6; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += polyN[i];
        tmp[i + 6] -= polyN[i];
    }
    cur_deg += 6;
    for (i = 0; i <= cur_deg; ++i) polyN[i] = tmp[i];

    /* multiply by (1-x⁸) */
    for (i = 0; i <= cur_deg + 8; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += polyN[i];
        tmp[i + 8] -= polyN[i];
    }
    cur_deg += 8;
    for (i = 0; i <= cur_deg; ++i) polyN[i] = tmp[i];

    /* multiply by (1-x¹²) */
    for (i = 0; i <= cur_deg + 12; ++i) tmp[i] = 0;
    for (i = 0; i <= cur_deg; ++i) {
        tmp[i]     += polyN[i];
        tmp[i + 12] -= polyN[i];
    }
    cur_deg += 12;
    for (i = 0; i <= cur_deg; ++i) polyN[i] = tmp[i];

    /* copy coefficients to N_coef */
    for (i = 0; i <= 28; ++i) N_coef[i] = polyN[i];
}

/* ------------------------------------------------------------------ */
/*  Memoisation cache                                                */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* resize the cache to hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
    cache_cap = new_cap;
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/* ------------------------------------------------------------------ */
/*  Sequence function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static int init_flag = 0;
    if (!init_flag) {
        init_coeffs();
        init_flag = 1;
    }

    size_t idx = (size_t)n;
    if (idx < cache_size) return cache[idx];

    size_t need = idx + 1;
    ensure_capacity(need);

    for (size_t i = cache_size; i < need; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 28; ++k) {
            if (i < (size_t)k) continue;
            int ck = D_coef[k];
            if (ck == 0) continue;
            /* ck may be negative; cast to seq_t preserves sign for signed types */
            seq_t prod = cache[i - k] * (seq_t)ck;
            sum += prod;
        }
        seq_t ncoeff = (i <= 28) ? (seq_t)N_coef[i] : (seq_t)0;
        cache[i] = ncoeff - sum;
    }
    cache_size = need;
    return cache[idx];
}
