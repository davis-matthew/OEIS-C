/*
 * A378276  Cogrowth sequence of the 20-element dicyclic group Dic20 = <S,T |
 *          S^5, T^4, STST^3>.
 * https://oeis.org/A378276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the cogrowth sequence of Dic20. It satisfies the
 *            recurrence a(n) = a(n-2) + 2*a(n-3) + 3*a(n-4) + 2*a(n-5) +
 *            10*a(n-6) + 8*a(n-7) + 16*a(n-8) – 16*a(n-9), for n ≥ 10. The
 *            first ten terms (index 0‑9) are obtained from the generating
 *            function coefficients: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached sequence values */
static idx_t  computed = 0;        /* number of values already computed */

/* coefficients of numerator N(x) and denominator D(x) polynomials */
static const int p[10] = {  1, 0,-1,-2,-2,-2,-5,-3,-6, 4 }; /* N(x) coeffs, 0…9 */
static const int d[10] = {  1, 0,-1,-2,-3,-2,-10,-8,-16,16 };/* D(x) coeffs, 0…9 */

/* ensure that the cache has space for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* allocate for n+1 elements */
        cache = (seq_t *)realloc(NULL, (size_t)(n + 1) * sizeof(seq_t));
        return;
    }
    if ((size_t)computed > n) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();        /* allocation failure */
    cache = tmp;
    /* we don't need to touch newly allocated elements; they'll be set in the loop */
}

/* public function required by the build */
seq_t a(idx_t n)
{
    /* allocate / extend cache if necessary */
    if (n >= computed) {
        ensure_cache(n);
        /* compute new values from computed up to n */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t val = 0;
            /* contributions from numerator if i < 10 */
            if (i < 10) val = (seq_t)p[i];
            /* add terms with negative denominator coefficients (k=2..8) */
            if (i >= 2) val += (seq_t)(-d[2]) * cache[(size_t)(i - 2)];
            if (i >= 3) val += (seq_t)(-d[3]) * cache[(size_t)(i - 3)];
            if (i >= 4) val += (seq_t)(-d[4]) * cache[(size_t)(i - 4)];
            if (i >= 5) val += (seq_t)(-d[5]) * cache[(size_t)(i - 5)];
            if (i >= 6) val += (seq_t)(-d[6]) * cache[(size_t)(i - 6)];
            if (i >= 7) val += (seq_t)(-d[7]) * cache[(size_t)(i - 7)];
            if (i >= 8) val += (seq_t)(-d[8]) * cache[(size_t)(i - 8)];
            /* subtract term with positive coefficient (k=9) */
            if (i >= 9) val -= (seq_t)d[9] * cache[(size_t)(i - 9)];
            cache[(size_t)i] = val;
        }
        computed = n + 1;
    }
    return cache[(size_t)n];
}
