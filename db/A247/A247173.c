/*
 * A247173  Expansion of g.f. (-1)/(2*x^2+2*x) +(-2*x^3-3*x^2+1) /
 *          (sqrt(x^4+4*x^3-2*x^2-4*x+1)*(2*x^2+2*x)).
 * https://oeis.org/A247173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed from the D‑finite recurrence * a(0)=1, a(1)=1,
 *            a(2)=7, a(3)=25, a(4)=101 * a(n) = ((9n²-3n-4)a(n-1) +
 *            2(9n²-21n+8)a(n-2) + 2(-3n²+n+8)a(n-3) + (-15n²+53n-12)a(n-4) *
 *            -(3n-1)(n-4)a(n-5)) / ((n+1)(3n-4)) for n ≥ 5
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#  include <new>
#endif
#include <stdbool.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed = 0;
static bool initialized = false;

static void ensure_capacity(idx_t max_index)
{
    if (cache_size > max_index) return;
    idx_t new_size = cache_size > 0 ? cache_size * 2 : 32;
    while (new_size <= max_index) new_size *= 2;
#ifdef __cplusplus
    seq_t *new_cache = (seq_t*)::operator new(new_size * sizeof(seq_t));
    /* copy existing elements */
    for (idx_t i = 0; i < cache_size; ++i) {
        new (new_cache + i) seq_t(cache[i]);   // copy-construct
    }
    /* destroy old elements */
    for (idx_t i = 0; i < cache_size; ++i) {
        cache[i].~seq_t();
    }
    ::operator delete(cache);
    /* default-construct new elements */
    for (idx_t i = cache_size; i < new_size; ++i) {
        new (new_cache + i) seq_t();
    }
    cache = new_cache;
#else
    cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    for (idx_t i = cache_size; i < new_size; ++i) {
        cache[i] = 0;   /* zero-initialize for primitive types */
    }
#endif
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Initialize base terms once */
    if (!initialized) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)101;
        computed = 5;
        initialized = true;
    }

    if (n < computed) return cache[n];

    ensure_capacity(n);

    for (idx_t i = computed; i <= n; ++i) {
        if (i < 5) continue;   /* base terms already set */

        /* Convert index to seq_t for coefficient calculations */
        seq_t n_seq = (seq_t)i;
        seq_t n2 = n_seq * n_seq;

        /* Coefficients (absolute values for negative terms) */
        seq_t A1 = (seq_t)9 * n2 - (seq_t)3 * n_seq - (seq_t)4;
        seq_t A2 = (seq_t)2 * ((seq_t)9 * n2 - (seq_t)21 * n_seq + (seq_t)8);
        seq_t absA3 = (seq_t)2 * ((seq_t)3 * n2 - (seq_t)1 * n_seq - (seq_t)8);
        seq_t absA4 = (seq_t)15 * n2 - (seq_t)53 * n_seq + (seq_t)12;
        seq_t coeff5 = (seq_t)(3 * i - 1) * (i - 4);

        /* Term calculations */
        seq_t term1 = cache[i-1] * A1;
        seq_t term2 = cache[i-2] * A2;
        seq_t term3 = cache[i-3] * absA3;  /* subtract later */
        seq_t term4 = cache[i-4] * absA4;  /* subtract later */
        seq_t term5 = cache[i-5] * coeff5; /* subtract later */

        /* Numerator: term1 + term2 - term3 - term4 - term5 */
        seq_t numerator = term1 + term2;
        numerator -= term3;
        numerator -= term4;
        numerator -= term5;

        /* Denominator */
        seq_t denom = (seq_t)(i + 1) * (seq_t)(3 * i - 4);

        cache[i] = numerator / denom;
    }

    computed = n + 1;
    return cache[n];
}
