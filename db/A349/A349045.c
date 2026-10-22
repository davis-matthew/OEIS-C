/*
 * A349045  G.f. A(x) satisfies: A(x) = 1 / (1 + x - 2 * x * A(-3*x)).
 * https://oeis.org/A349045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -a(n-1) + 2 * Σ_{k=0}^{n-1} (-3)^k * a(k) *
 *            a(n-k-1) for n ≥ 1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/*  cache for sequence values  */
static seq_t *seq_cache  = NULL;   /* sequence values a(0)..a(n)   */
static size_t seq_cap    = 0;      /* allocated size                */
static size_t seq_len    = 0;      /* number of elements computed   */

/*  cache for powers (-3)^k  */
static seq_t *pow_cache  = NULL;   /* (-3)^k for k = 0..n          */
static size_t pow_cap    = 0;      /* allocated size                */
static size_t pow_len    = 0;      /* number of elements computed   */

/*  expand a cache to hold at least (n+1) elements  */
static void ensure_seq_cache(idx_t n)
{
    if (n >= seq_cap) {
        seq_cap = n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
}

static void ensure_pow_cache(idx_t n)
{
    if (n >= pow_cap) {
        pow_cap = n + 1;
        pow_cache = (seq_t *)realloc(pow_cache, pow_cap * sizeof(seq_t));
    }
}

/*  compute sequence up to n  */
static void compute_until(idx_t n)
{
    /*  allocate and grow the caches  */
    ensure_seq_cache(n);
    ensure_pow_cache(n);

    /*  ensure we have the powers (-3)^k  */
    if (pow_len == 0) {           /* initialise 0‑th power  */
        pow_cache[0] = 1;
        pow_len = 1;
    }
    for (idx_t i = pow_len; i <= n; ++i) {
        pow_cache[i] = (-3) * pow_cache[i-1];
    }
    if (n >= pow_len) pow_len = n + 1;

    /*  initialise the first term  */
    if (seq_len == 0) {
        seq_cache[0] = 1;
        seq_len = 1;
    }

    /*  compute the remaining terms  */
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            /*   term = (-3)^k * a(k) * a(i-k-1)   */
            seq_t prod = seq_cache[k] * seq_cache[i - k - 1];
            seq_t term = pow_cache[k] * prod;
            sum += term;
        }
        seq_cache[i] = -seq_cache[i-1] + ((seq_t)2) * sum;
    }
    if (n >= seq_len) seq_len = n + 1;
}

/*  public function required by the build  */
seq_t a(idx_t n)
{
    if (n >= seq_len) {          /* compute missing values  */
        compute_until(n);
    }
    return seq_cache[n];
}
