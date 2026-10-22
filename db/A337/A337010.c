/*
 * A337010  a(n) = exp(-1/2) * Sum_{k>=0} (2*k + 3)^n / (2^k * k!).
 * https://oeis.org/A337010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp(-1/2) * Sum_{k>=0} (2*k + 3)^n / (2^k * k!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used (valid for n >= 0):
 *   a(0) = 1
 *   a(1) = 4
 *   a(n) = 4 * a(n-1) + Sum_{j=0}^{n-2} binomial(n-1, j) * 2^{n-1-j} * a(j)   (n ≥ 2)  */

#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system.
   idx_t is an unsigned integer type (e.g. uint64_t).  seq_t may be
   unsigned 8/16/32/64-bit, unsigned __int128, or an arbitrary‑precision
   integer class.  All arithmetic uses the operators +, -, *, /, % and
   comparisons, suitable for seq_t. */

static seq_t *cache = NULL;        /* cache[n] = a(n) */
static size_t cache_len = 0;       /* allocated size */
static idx_t   max_cached = 0;     /* maximum n for which cache[n] is valid */

static seq_t *pow2_cache = NULL;   /* pow2_cache[e] = 2^e */
static size_t pow2_len = 0;        /* allocated size of pow2_cache */

/* Ensure that pow2_cache contains 2^e for every e <= max_e.  */
static void
ensure_pow2(idx_t max_e)
{
    if ((pow2_cache != NULL) && (pow2_len > max_e))
        return;                           /* already sufficient */

    size_t new_len = (size_t)max_e + 1;    /* index 0 .. max_e inclusive */

    if (pow2_cache == NULL) {
        pow2_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        pow2_cache[0] = (seq_t)1;
        pow2_len = 1;
    } else if (new_len > pow2_len) {
        pow2_cache = (seq_t *)realloc(pow2_cache, new_len * sizeof(seq_t));
    }

    for (size_t i = pow2_len; i < new_len; ++i) {
        pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
    }
    pow2_len = new_len;
}

/* Ensure that cache can hold values up to new_n (inclusive) and
   compute any missing terms using the recurrence.  */
static void
ensure_cache(idx_t new_n)
{
    if ((cache != NULL) && (cache_len > new_n))
        return;                      /* already sufficient */

    size_t new_len = (size_t)new_n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache_len = new_len;

    /* Initialise base values if they are required.  */
    if (max_cached == 0) {
        cache[0] = (seq_t)1;
    }
    if (max_cached < 1 && new_n >= 1) {
        cache[1] = (seq_t)4;
    }

    /* Ensure pow2_cache holds the powers of two needed in the sum.  */
    ensure_pow2(new_n);

    /* Compute any missing terms.  */
    for (idx_t n = max_cached + 1; n <= new_n; ++n) {
        seq_t sum = 0;
        if (n >= 2) {
            seq_t comb = 1;            /* C(n-1,0) */
            for (idx_t j = 0; j <= n - 2; ++j) {
                if (j > 0) {
                    /* Compute C(n-1, j) from C(n-1, j-1) */
                    comb = comb * (seq_t)(n - j) / (seq_t)j;
                }
                seq_t term = comb * pow2_cache[(size_t)(n - 1 - j)] * cache[j];
                sum += term;
            }
        }
        seq_t val = cache[n - 1] * (seq_t)4 + sum;
        cache[n] = val;
    }
    max_cached = new_n;
}

/* Public function.  */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
