/*
 * A266761  Growth series for affine Coxeter group (or affine Weyl group) D_6.
 * https://oeis.org/A266761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient sequence of the growth series of the
 *            affine Coxeter group D_6 (OEIS A266761). It satisfies the linear
 *            recurrence derived from the rational generating function G(t) =
 *            ∏_{i=1}^{6} (1 - t^{m_i+1}) / ((1-t)⋅∏_{i=1}^{6}(1 - t^{m_i}))
 *            with m_i = {1,3,5,7,9,5}. The recurrence coe...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 160, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static void init_polynomials(void);
static void ensure_cache(idx_t n);

enum { DEG = 36 };            /* degree of both N(t) and D(t) */

static int64_t Dcoeff[DEG + 1];    /* signed denominator coefficients */
static int64_t Ncoeff[DEG + 1];    /* signed numerator coefficients */
static seq_t *cache = 0;           /* memoisation cache for a(n) */
static size_t cache_size = 0;      /* allocated size of cache */
static size_t computed = 0;        /* number of terms already cached */
static int initialized = 0;

/* Multiply two small polynomials with signed coefficients.
   The result is written into res and the resulting degree is stored in *gd.   */
static void poly_mul(int64_t *res, const int64_t *a, size_t da,
                     const int64_t *b, size_t db, size_t *gd)
{
    size_t d = da + db;
    for (size_t i = 0; i <= d; ++i) res[i] = 0;
    for (size_t i = 0; i <= da; ++i)
        for (size_t j = 0; j <= db; ++j)
            res[i + j] += a[i] * b[j];
    *gd = d;
}

/* Initialise Dcoeff and Ncoeff once. */
static void init_polynomials(void)
{
    if (initialized) return;
    initialized = 1;

    /* Build D(t) = (1 - t)^6 * ∏(1 - t^m) ,  m = {1,3,5,5,7,9}. */
    int64_t poly[DEG + 1] = {0};
    size_t deg = 0;

    /* (1 - t)^6 */
    for (int i = 0; i <= 6; ++i) {
        /* binomial coefficient C(6,i) */
        int64_t c = 1;
        for (int j = 1; j <= i; ++j)
            c = c * (6 - j + 1) / j;
        poly[i] = (i % 2 == 0) ? c : -c;
    }
    deg = 6;

    /* factors (1 - t^m) */
    const int m[6] = {1, 3, 5, 5, 7, 9};
    for (int f = 0; f < 6; ++f) {
        int e = m[f];
        int64_t tmp[DEG + 1] = {0};
        size_t nd = deg + e;
        for (size_t i = 0; i <= nd && i <= (size_t)DEG; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= deg && i <= (size_t)DEG; ++i) {
            tmp[i] += poly[i];
            if (i + e <= (size_t)DEG)
                tmp[i + e] -= poly[i];
        }
        for (size_t i = 0; i <= nd && i <= (size_t)DEG; ++i) poly[i] = tmp[i];
        deg = nd;
    }
    for (size_t i = 0; i <= DEG; ++i) Dcoeff[i] = poly[i];

    /* Build N(t) = ∏(1 - t^{m_i+1}),  e = {2,4,6,6,8,10}. */
    int64_t polyN[DEG + 1] = {0};
    polyN[0] = 1;
    size_t degN = 0;
    const int eN[6] = {2, 4, 6, 6, 8, 10};
    for (int f = 0; f < 6; ++f) {
        int e = eN[f];
        int64_t tmpN[DEG + 1] = {0};
        size_t nd = degN + e;
        for (size_t i = 0; i <= nd && i <= (size_t)DEG; ++i) tmpN[i] = 0;
        for (size_t i = 0; i <= degN && i <= (size_t)DEG; ++i) {
            tmpN[i] += polyN[i];
            if (i + e <= (size_t)DEG)
                tmpN[i + e] -= polyN[i];
        }
        for (size_t i = 0; i <= nd && i <= (size_t)DEG; ++i) polyN[i] = tmpN[i];
        degN = nd;
    }
    for (size_t i = 0; i <= DEG; ++i) Ncoeff[i] = polyN[i];
}

/* Ensure that the cache has space for up to n+1 terms. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;
    size_t newcap = (cache_size == 0) ? 16 : cache_size;
    while (newcap <= (size_t)n + 1) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    for (size_t i = cache_size; i < newcap; ++i) cache[i] = 0;
    cache_size = newcap;
}

/* Public function. */
seq_t a(idx_t n)
{
    init_polynomials();
    ensure_cache(n);

    if (n < computed) return cache[n];

    for (size_t i = computed; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* coefficient from numerator */
        int64_t nc = (i <= (size_t)DEG) ? Ncoeff[i] : 0;
        if (nc > 0)
            sum_pos += (seq_t)nc;
        else if (nc < 0)
            sum_neg += (seq_t)(-nc);

        /* contributions from denominator */
        for (size_t k = 1; k <= (size_t)DEG; ++k) {
            if (i < k) continue;
            const seq_t term = cache[i - k];
            int64_t ck = Dcoeff[k];
            if (ck > 0)
                sum_neg += term * (seq_t)ck;
            else if (ck < 0)
                sum_pos += term * (seq_t)(-ck);
        }

        /* a(i) = sum_pos - sum_neg; the difference is guaranteed non‑negative. */
        cache[i] = sum_pos - sum_neg;
    }
    computed = n + 1;
    return cache[n];
}
