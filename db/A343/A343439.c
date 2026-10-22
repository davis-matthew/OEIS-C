/*
 * A343439  G.f.: 1 + 2^0*x/(1 + 2^1*x/(1 + 2^2*x/(1 + 2^3*x/(1 + 2^4*x/(1 +
 *          ...))))).
 * https://oeis.org/A343439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1; For n>1: a(n) = - sum_{k=1}^{n-1} 2^k * a(k) *
 *            a(n-k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_len = 0;

static seq_t *pow2_cache = NULL;
static idx_t pow2_len = 0;

static void ensure_pow2(idx_t n)
{
    if (pow2_len == 0) {
        pow2_cache = (seq_t*)malloc(sizeof(seq_t));
        pow2_cache[0] = (seq_t)1;
        pow2_len = 1;
    }
    while (pow2_len <= n) {
        pow2_cache = (seq_t*)realloc(pow2_cache, (pow2_len + 1) * sizeof(seq_t));
        pow2_cache[pow2_len] = pow2_cache[pow2_len - 1] * (seq_t)2;
        ++pow2_len;
    }
}

static void ensure_seq(idx_t n)
{
    if (seq_len > n) return;
    seq_cache = (seq_t*)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = seq_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            seq_cache[i] = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                ensure_pow2(k);
                seq_t term = pow2_cache[k] * seq_cache[k] * seq_cache[i - k];
                sum += term;
            }
            seq_cache[i] = -sum;
        }
    }
    seq_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_seq(n);
    return seq_cache[n];
}
