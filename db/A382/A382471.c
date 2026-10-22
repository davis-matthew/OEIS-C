/*
 * A382471  a(n) = Sum_{k=0..n} binomial(k+4,4) * binomial(2*k,2*n-2*k).
 * https://oeis.org/A382471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+4,4) * binomial(2*k,2*n-2*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the 20‑term linear recurrence obtained from the
   rational generating function:
   (1 - 2x - x^2 - 2x^3 + x^4)^5 * A(x) = polynomial of degree 20.
   The coefficients of the polynomial give a recurrence
     a(n) = Σ_{j=1..20} coeff[j] * a(n-j)
   for n ≥ 20.  The first 20 values are supplied explicitly.  The
   recurrence and the first values are used to compute arbitrary n.
   The routine is fully memoised: already computed values are stored
   in a static array and reused on later calls.
*/

#include <stdint.h>
#include <stdlib.h>

/* First 20 terms of the sequence */
static const seq_t init[20] = {
    (seq_t)1ULL,
    (seq_t)5ULL,
    (seq_t)20ULL,
    (seq_t)125ULL,
    (seq_t)610ULL,
    (seq_t)2611ULL,
    (seq_t)10815ULL,
    (seq_t)42610ULL,
    (seq_t)161005ULL,
    (seq_t)590155ULL,
    (seq_t)2106362ULL,
    (seq_t)7348265ULL,
    (seq_t)25141430ULL,
    (seq_t)84569395ULL,
    (seq_t)280246795ULL,
    (seq_t)916465742ULL,
    (seq_t)2961805180ULL,
    (seq_t)9470735650ULL,
    (seq_t)29994694130ULL,
    (seq_t)94172180660ULL
};

/* Recurrence coefficients: coeff[j] = -(q_j) where q(x) is (1-2x-x^2-2x^3+x^4)^5 */
static int64_t coeff[21] = {0};
static int coeff_ready = 0;

static void compute_coeffs(void)
{
    if (coeff_ready) return;
    const int base_deg = 4;
    const long long base[5] = {1, -2, -1, -2, 1};   /* coefficients of 1-2x-x^2-2x^3+x^4 */
    __int128 poly[21]   = {0};
    __int128 next[21]   = {0};
    int cur_deg = 0;

    poly[0] = 1;   /* start with 1 */

    for (int e = 0; e < 5; ++e) {
        for (int i = 0; i <= cur_deg; ++i)
            for (int j = 0; j <= base_deg; ++j)
                next[i + j] += poly[i] * (__int128)base[j];

        for (int i = 0; i <= 20; ++i) {
            poly[i] = next[i];
            next[i] = 0;
        }
        cur_deg = cur_deg + base_deg;
        if (cur_deg > 20) cur_deg = 20;
    }

    for (int i = 1; i <= 20; ++i)
        coeff[i] = -(int64_t)poly[i];   /* coeff[j] = -q_j */

    coeff_ready = 1;
}

/* Cache of computed a(n) */
static seq_t *cache     = NULL;   /* array holding computed values */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of valid entries */

/* Ensure cache has capacity at least 'new_cap' entries. */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t new_size = cache_cap ? cache_cap : 20;
    while (new_size < new_cap) new_size <<= 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_cap = new_size;
}

/* Initialise cache with the first 20 terms if it hasn't been done yet. */
static void init_cache(void)
{
    if (cache_len != 0) return;
    ensure_cache_cap(20);
    for (int i = 0; i < 20; ++i)
        cache[i] = init[i];
    cache_len = 20;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 20) return init[(size_t)n];

    compute_coeffs();
    init_cache();

    /* Extend cache up to n */
    if ((size_t)n >= cache_len) {
        ensure_cache_cap((size_t)n + 1);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* Compute a(i) using the recurrence */
            seq_t pos = 0;   /* sum of terms with positive coeff */
            seq_t neg = 0;   /* sum of terms with negative coeff */
            for (int j = 1; j <= 20; ++j) {
                seq_t prod = cache[i - j];
                prod = prod * (seq_t)(coeff[j] < 0 ? -coeff[j] : coeff[j]);
                if (coeff[j] < 0)
                    neg += prod;
                else
                    pos += prod;
            }
            cache[i] = pos - neg;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
