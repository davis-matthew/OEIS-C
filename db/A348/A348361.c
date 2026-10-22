/*
 * A348361  a(n) = Sum_{k=1..n} k^(k'), where ' is the arithmetic derivative.
 * https://oeis.org/A348361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(k') where k' is the arithmetic derivative
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Arithmetic derivative of an integer n.
   For n ≤ 1 the derivative is 0. */
static uint64_t arithmetic_derivative(uint64_t n) {
    if (n <= 1) return 0;
    uint64_t result = 0;
    uint64_t orig   = n;
    uint64_t temp   = n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* term = orig * e / p, computed in 128‑bit to avoid overflow */
            unsigned __int128 term = (unsigned __int128)orig * e / p;
            result += (uint64_t)term;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        unsigned __int128 term = (unsigned __int128)orig / temp;
        result += (uint64_t)term;
    }
    return result;
}

/* OEIS sequence A348361 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */

    /* static storage for a prefix of the cumulative sum */
    static seq_t *cum = NULL;
    static size_t cap = 0;        /* capacity of cum array */
    static idx_t  max_computed = 0; /* largest n computed so far */

    /* initialise storage on first call */
    if (cum == NULL) {
        cum = seq_alloc_new_array(1);
        cap = 1;
        cum[0] = 0;
        max_computed = 0;
    }

    /* grow storage if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cum = seq_alloc_new_array(new_cap);
        for (size_t i = 0; i < cap; ++i) new_cum[i] = cum[i];
        seq_free_array(cum);
        cum = new_cum;
        cap = new_cap;
    }

    /* append missing terms */
    if (n > max_computed) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            uint64_t d   = arithmetic_derivative((uint64_t)k);
            seq_t term   = pow_uint((uint64_t)k, (idx_t)d);
            cum[k] = cum[k - 1] + term;
        }
        max_computed = n;
    }

    return cum[(size_t)n];
}
