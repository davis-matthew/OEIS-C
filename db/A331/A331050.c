/*
 * A331050  Positive integers whose number of factorizations into factors > 1
 *          (A001055) is odd.
 * https://oeis.org/A331050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer m such that A001055(m) is odd.
 *            Computation is performed by a recursive parity check of the
 *            multiplicative partition function A001055
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   parity_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/parity_f.h"

/* Storage for terms already found */
static seq_t *terms_buf = NULL;
static idx_t terms_count = 0;
static idx_t terms_cap   = 0;
static idx_t next_candidate = 1;

/* Ensure that at least 'required' terms are stored in terms_buf. */
static void ensure_terms(idx_t required) {
    while (terms_count < required) {
        if (parity_f(next_candidate, 2) == 1) {
            if (terms_count == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms_buf, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms_buf = tmp;
                terms_cap = new_cap;
            }
            terms_buf[terms_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* fallback for undefined offset */
    ensure_terms(n);
    return terms_buf[n - 1];
}
