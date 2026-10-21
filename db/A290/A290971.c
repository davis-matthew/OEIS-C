/*
 * A290971  Write x/(1-x) in the form Sum_{j>=1} a(j)*x^j/(1+a(j)*x^j).
 * https://oeis.org/A290971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; for n > 1: a(n) = 1 - Σ_{j|n, j<n} (-1)^{(n/j)-1} ·
 *            a(j)^{n/j} Derived from Σ_{j|n} (-1)^{(n/j)-1} · a(j)^{n/j} = 1
 * Range:     i8 n <= 15, i16 n <= 23, i32 n <= 47, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t *seq_cache = NULL;
static unsigned char *seq_cached = NULL;
static size_t seq_cache_cap = 0;

/* Ensure the cache can store values up to n.  Newly added entries are marked as not cached. */
static void ensure_seq_cache(idx_t n)
{
    if ((size_t)n >= seq_cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cached = (unsigned char *)realloc(seq_cached, new_cap * sizeof(unsigned char));
        for (size_t i = seq_cache_cap; i < new_cap; ++i) seq_cached[i] = 0;
        seq_cache_cap = new_cap;
    }
}

/* Recursively (or iteratively) compute a(n) with memoisation. */
static seq_t a_of(idx_t n)
{
    ensure_seq_cache(n);
    if (seq_cached[n]) return seq_cache[n];
    seq_t result;

    if (n == 1) {
        result = (seq_t)1;
    } else {
        seq_t sum = (seq_t)0;
        for (idx_t i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                /* divisor d1 = i */
                idx_t d1 = i;
                if (d1 < n) {
                    idx_t m1 = n / d1;   /* exponent */
                    seq_t val1 = a_of(d1);
                    seq_t pow1 = big_pow(val1, (seq_t)m1);
                    if ((m1 & 1) == 0) pow1 = -pow1;   /* (-1)^{m-1} */
                    sum += pow1;
                }
                /* divisor d2 = n / i, distinct from d1 */
                idx_t d2 = n / i;
                if (d2 != d1 && d2 < n) {
                    idx_t m2 = i;        /* n / d2 equals i */
                    seq_t val2 = a_of(d2);
                    seq_t pow2 = big_pow(val2, (seq_t)m2);
                    if ((m2 & 1) == 0) pow2 = -pow2;
                    sum += pow2;
                }
            }
        }
        result = (seq_t)1 - sum;
    }

    seq_cache[n] = result;
    seq_cached[n] = 1;
    return result;
}

/* Public entry point called by the framework. */
seq_t a(idx_t n)
{
    return a_of(n);
}
