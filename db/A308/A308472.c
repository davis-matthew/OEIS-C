/*
 * A308472  Numbers that are divisible by the sum of the digits of the product
 *          of their digits.
 * https://oeis.org/A308472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 4490, u32 n <= 6253, u64 n <= 6253,
 *            u128 n <= 6253, big any n
 * Helpers:   digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A308472 by brute‑force search.
   The sequence is defined as the set of zeroless integers n such that
   (product of the digits of n) % (sum of the digits of that product) == 0. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* 0‑based cache: terms[0] == a(1) */
    static seq_t *terms = NULL;
    static idx_t len   = 0;      /* number of stored terms */
    static idx_t cap   = 0;      /* allocated capacity */

    if (n == 0)
        return 0;                 /* undefined for offset 0 */

    /* Extend cache until it contains at least n terms. */
    while (len < n)
    {
        seq_t candidate = (len == 0) ? (seq_t)1 : terms[len - 1] + 1;

        /* Find the next qualifying number. */
        for (;;)
        {
            /* Skip numbers containing a zero digit. */
            seq_t tmp = candidate;
            bool has_zero = false;
            while (tmp > 0)
            {
                if ((tmp % 10) == 0) {   /* zero digit */
                    has_zero = true;
                    break;
                }
                tmp /= 10;
            }
            if (!has_zero)
            {
                seq_t prod = digit_product(candidate);
                seq_t sum  = digit_sum(prod);
                if (sum != (seq_t)0 && (candidate % sum) == (seq_t)0)
                    break;                      /* candidate accepted */
            }
            ++candidate;
        }

        /* Store the new term, expanding the array if necessary. */
        if (len == cap)
        {
            idx_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)                 /* allocation failed */
            {
                free(terms);
                terms = NULL;
                return 0;
            }
            terms = tmp;
            cap   = new_cap;
        }
        terms[len++] = candidate;
    }

    return terms[n - 1];
}
