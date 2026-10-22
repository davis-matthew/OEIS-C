/*
 * A343914  Riesel problem in base 3: a(n) is the smallest k >= 0 such that
 *          (2*n)*3^k-1 is prime, or -1 if no such k exists.
 * https://oeis.org/A343914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest k ≥ 0 such that (2*n)*3^k – 1 is prime, or –1 if
 *            no such k exists
 * Range:     i8 n <= 10, i16 n <= 62, i32 n <= 62, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   is_prime_big, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

/* Cache for powers of 3 */
static seq_t *pow3 = NULL;
static size_t pow3_cap = 0;

/* Ensure that pow3 array contains 3^k for all k < k_needed. */
static void ensure_pow3(size_t k_needed) {
    if (k_needed < pow3_cap) return;
    size_t new_cap = k_needed + 1;
    seq_t *new_pow3 = seq_realloc(pow3, pow3_cap, new_cap);
    pow3 = new_pow3;
    /* initialise new entries */
    if (pow3_cap == 0) {
        pow3[0] = 1;
        pow3_cap = 1;
    }
    for (size_t i = pow3_cap; i < new_cap; ++i) {
        pow3[i] = pow3[i - 1] * 3;
    }
    pow3_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return -1;          /* sequence defined for n ≥ 1 */
    seq_t base = (seq_t)n * 2;      /* (2*n) as seq_t */

    /* Search for the smallest k with prime */
    for (size_t k = 0;; ++k) {
        ensure_pow3(k);
        seq_t val = seq_mul(base, pow3[k]);  /* (2*n)*3^k */
        val -= 1;                            /* –1 */
        if (is_prime_big(val))
            return (seq_t)k;
    }
}
