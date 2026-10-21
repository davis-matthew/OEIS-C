/*
 * A244748  Numbers k such that (product of digits of k)^2 + 1 is prime.
 * https://oeis.org/A244748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 768, u32 n <= 768, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244748: Numbers k such that (product of digits of k)^2 + 1 is prime. */

/* The function uses a simple incremental search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_product.h"
#include "../utils/seq_mul.h"
#include "../utils/is_prime_big.h"

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static storage for the term list and the generation state. */
    static seq_t *terms = NULL;      /* array of found terms */
    static size_t capacity = 0;      /* allocated capacity of terms */
    static size_t size = 0;          /* number of terms stored */
    static seq_t next_k = 1;         /* next integer to test */

    /* Ensure we have at least n terms precomputed. */
    size_t target = (size_t)n;
    while (size < target) {
        seq_t k = next_k;
        seq_t prod = digit_product(k);
        seq_t val = seq_mul(prod, prod);  /* (product of digits)^2 */
        val += 1;                         /* + 1 */

        if (is_prime_big(val)) {
            /* Grow the array if necessary. */
            if (size == capacity) {
                size_t new_cap = (capacity == 0) ? 128 : capacity * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failed */
                terms = tmp;
                capacity = new_cap;
            }
            terms[size++] = k;
        }
        next_k++;
    }

    return terms[n - 1];
}
