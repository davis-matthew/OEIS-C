/*
 * A283407  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 637", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A283407
 *
 * a(n) = p[n] + 2*a(n-1) + a(n-2) - 2*a(n-3)  for n >= 3,
 * where p[n] are the coefficients of the numerator polynomial
 *   P(x) = -3221225472*x^33 + 536870912*x^32 + 3221225472*x^31
 *          -536870912*x^30 + 256*x^13 + 640*x^12 - 896*x^11
 *          -512*x^10 + 640*x^9 - 128*x^8 + 32*x^7 - 16*x^5
 *          -8*x^4 + 12*x^3 - 2*x^2 - x + 1
 *
 * For n > 33 the polynomial term vanishes, yielding the
 * homogeneous recurrence
 *   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3).
 *
 * The first 34 terms are obtained from the generating function
 * and are cached for fast evaluation of larger indices.
 */

#include <stdlib.h>
#include <stdint.h>

/*--
 * coeffs p[0] .. p[33] : numerator coefficients of P(x)
 * (p[n] is the coefficient of x^n)
 *--*/
static const int64_t p[34] = {
    1LL,          /* x^0   */
    -1LL,         /* x^1   */
    -2LL,         /* x^2   */
    12LL,         /* x^3   */
    -8LL,         /* x^4   */
    -16LL,        /* x^5   */
    0LL,          /* x^6   */
    32LL,         /* x^7   */
    -128LL,       /* x^8   */
    640LL,        /* x^9   */
    -512LL,       /* x^10  */
    -896LL,       /* x^11  */
    640LL,        /* x^12  */
    256LL,        /* x^13  */
    0LL, 0LL, 0LL, 0LL, 0LL, /* 14‑18  */
    0LL, 0LL, 0LL, 0LL,       /* 19‑22  */
    0LL, 0LL,                 /* 23‑24  */
    0LL, 0LL,                 /* 25‑26  */
    0LL, 0LL,                 /* 27‑28  */
    0LL,                      /* 29     */
    -536870912LL,             /* x^30  */
    3221225472LL,             /* x^31 */
    536870912LL,              /* x^32 */
    -3221225472LL             /* x^33 */
};

/* helper: add a signed 64‑bit constant to an unsigned seq_t value  */
static inline seq_t add_signed(seq_t v, int64_t s)
{
    return (s >= 0) ? (v + (seq_t)s) : (v - (seq_t)(-s));
}

/* cache for the first 34 terms */
static seq_t init34[34];
static int   init_done = 0;

/* build the initial 34 terms from the generating function */
static void compute_init(void)
{
    seq_t tmp;
    init34[0] = (seq_t)p[0];

    /* a1 = p1 + 2*a0 */
    tmp = init34[0];
    tmp <<= 1;
    init34[1] = add_signed(tmp, p[1]);

    /* a2 = p2 + 2*a1 + a0 */
    tmp = init34[1];
    tmp <<= 1;
    tmp += init34[0];
    init34[2] = add_signed(tmp, p[2]);

    /* a3 = p3 + 2*a2 + a1 - 2*a0 */
    tmp = init34[2];
    tmp <<= 1;
    tmp += init34[1];
    tmp -= init34[0] << 1;
    init34[3] = add_signed(tmp, p[3]);

    /* a[n] = p[n] + 2*a[n-1] + a[n-2] - 2*a[n-3]   for 4 <= n <= 33 */
    for (unsigned i = 4; i < 34; ++i) {
        tmp = init34[i-1];
        tmp <<= 1;
        tmp += init34[i-2];
        tmp -= init34[i-3] << 1;
        init34[i] = add_signed(tmp, p[i]);
    }
    init_done = 1;
}

/* cache for values beyond the first 34 terms */
static seq_t *cache      = NULL;
static idx_t  cache_cap  = 34;   /* capacity of cache array */
static idx_t  cached_max = 33;  /* largest index computed so far */

static void ensure_cache(idx_t n)
{
    if (n <= cached_max)
        return;

    /* grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);                  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new terms using the homogeneous recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t t = cache[i-1];
        t <<= 1;                 /* 2*a(i-1) */
        t += cache[i-2];          /* + a(i-2) */
        t -= cache[i-3] << 1;     /* - 2*a(i-3) */
        cache[i] = t;
    }
    cached_max = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (!init_done)
        compute_init();

    /* Initialise the cache on first use */
    if (!cache) {
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache)
            exit(1);
        for (unsigned i = 0; i < 34; ++i)
            cache[i] = init34[i];
    }

    if (n <= cached_max)
        return cache[n];

    ensure_cache(n);
    return cache[n];
}
