/*
 * A356751  Positive integers m such that x^2 - x + m contains more than m/2
 *          prime numbers for x = 1, 2, ..., m.
 * https://oeis.org/A356751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 14, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A356751 by brute‑force search. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_used = 0;
    static idx_t next_m = 1;  /* the next m to test */

    while (terms_used < n) {
        seq_t count = 0;
        for (idx_t x = 1; x <= next_m; ++x) {
            seq_t val = (seq_t)x * (seq_t)x - (seq_t)x + (seq_t)next_m;
            if (is_prime_big(val))
                ++count;
        }
        if (count > next_m / 2) {
            if (terms_used == terms_cap) {
                terms_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) {    /* allocation failed */
                    exit(EXIT_FAILURE);
                }
                terms = tmp;
            }
            terms[terms_used++] = next_m;
        }
        ++next_m;
    }
    return terms[n - 1];
}
