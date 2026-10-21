/*
 * A253176  Least k>=0 such that both 3n*2^k+1 and 3n*2^k-1 are primes, or -1
 *          if no such k exists.
 * https://oeis.org/A253176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more, hard
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 20, i32 n <= 20, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the least k≥0 such that both 3n·2^k+1 and 3n·2^k–1 are primes;
   if no such k exists (within a reasonable bound) the function returns –1. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

static const idx_t MAX_K = 70;   /* a safe upper bound for the search */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;   /* outside sequence bounds */

    /* base = 3 * n */
    seq_t base = (seq_t)3 * (seq_t)n;

    for (idx_t k = 0; k <= MAX_K; ++k) {
        /* val = base * 2^k */
        seq_t val = seq_mul(base, pow2(k));

        seq_t left  = val + 1;
        seq_t right = val - 1;

        if (is_prime_big(left) && is_prime_big(right)) {
            return (seq_t)k;
        }
    }
    return (seq_t)-1;   /* no suitable k found within the bound */
}
