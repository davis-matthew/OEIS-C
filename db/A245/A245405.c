/*
 * A245405  Number A(n,k) of endofunctions on [n] such that no element has a
 *          preimage of cardinality k; square array A(n,k), n>=0, k>=0, read
 *          by antidiagonals.
 * https://oeis.org/A245405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sequence A245405 – number of endofunctions on [n] such
 *            that no element has a preimage of cardinality k. The square
 *            array A(n,k) is read by antidiagonals: a(0) = 1, a(1) = 1, a(2)
 *            = 1, a(3) = 1, a(4) = 0, a(5) = 2, …
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 100, u64 n <= 226,
 *            u128 n <= 523, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- integer square‑root (floor) on unsigned 64‑bit integers -------------------
 * Only basic arithmetic operations are used, no floating point or <math.h>.
 */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    idx_t best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { best = 0; low = 1; continue; }
        if (mid <= n / mid) { best = mid; low = mid + 1; }
        else { high = mid - 1; }
    }
    return best;
}

/* ----- cache factorials ---------------------------------------------------------
 * get_factorial(n) returns n! using a growable static array.
 */
static seq_t *fact_cache = NULL;   /* factorial cache (0! … n!)          */
static idx_t fact_limit = 0;        /* current cache size                  */

static seq_t get_factorial(idx_t n)
{
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) return 0;
        fact_cache[0] = 1;
        fact_limit = 1;
    }
    if (n >= fact_limit) {
        size_t new_lim = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(fact_cache, new_lim * sizeof(seq_t));
        if (!new_cache) return 0;
        for (idx_t i = fact_limit; i <= n; ++i)
            new_cache[i] = new_cache[i - 1] * (seq_t)i;
        fact_cache = new_cache;
        fact_limit = new_lim;
    }
    return fact_cache[n];
}

/* ----- fast exponentiation (base^exp) ---------------------------------------
 * Uses exponentiation by squaring; exp fits in idx_t.
 */
static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* ----- compute A(n,k) -------------------------------------------------------
 * Uses inclusion–exclusion:
 *   A(n,k) = Σ (-1)^j * C(n,j) * (n! / ((k!)^j * (n-jk)!)) * (n-j)^(n-jk)
 */
static seq_t compute_A(idx_t n, idx_t k)
{
    /* Special cases: k = 0 gives surjective functions, k > n gives all functions. */
    if (k == 0) return get_factorial(n);
    if (k > n) return pow_small((seq_t)n, n);

    idx_t maxj = n / k;
    seq_t fact_n = get_factorial(n);
    seq_t fact_k = get_factorial(k);
    seq_t result = 0;

    for (idx_t j = 0; j <= maxj; ++j) {
        idx_t exp = n - j * k;                     /* exponent for (n-j) */
        /* Binomial coefficient C(n,j) = n! / (j! (n-j)!) */
        seq_t bin = fact_n / (get_factorial(j) * get_factorial(n - j));
        seq_t fact_exp = get_factorial(exp);
        seq_t denom = pow_small(fact_k, j) * fact_exp;
        /* X = n! / ((k!)^j * (n-jk)!) is integer */
        seq_t X = fact_n / denom;
        seq_t pow_n = pow_small((seq_t)(n - j), exp);
        seq_t term = bin * X * pow_n;           /* integer term */
        if (j & 1)
            result -= term;
        else
            result += term;
    }
    return result;
}

/* ----- public function required by the specification -----------------------
 * Computes the sequence value at position idx (0‑based).
 */
seq_t a(idx_t idx)
{
    /* Find diagonal t such that t*(t+1)/2 <= idx < (t+1)*(t+2)/2 */
    idx_t root = isqrt_u64(8ULL * idx + 1);
    idx_t t = (root - 1) / 2;
    idx_t first_term = t * (t + 1) / 2;
    idx_t p = idx - first_term;                   /* position on diagonal */
    idx_t n = p;                                  /* row index      */
    idx_t k = t - p;                               /* column index   */
    return compute_A(n, k);
}
