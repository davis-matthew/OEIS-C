/*
 * A366140  Fixed points of the binary rotations A336953 and A366139: numbers
 *          k >= 0 such that A336953(k) = A366139(k) = k.
 * https://oeis.org/A366140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th nonnegative integer k such that the minimal *
 *            period of its binary representation (A302291(k)) divides k. *
 *            This implements OEIS A366140
 * Range:     u8 n <= 50, u16 n <= 4465, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Check if a number k satisfies the condition A302291(k) | k.
 * A302291(k) is the minimal period of the binary string of k under
 * cyclic rotation. */
static bool is_term(seq_t k)
{
    /* 0 is a fixed point by definition. */
    if (k == 0)
        return true;

    /* Length of the binary representation (number of bits). */
    idx_t L = bitlen_seq_t(k);

    /* Build a mask with L ones:  (1 << L) - 1, but constructed
     * safely for all widths. */
    seq_t mask = 0;
    for (idx_t i = 0; i < L; ++i)
        mask = (mask << 1) | 1;

    /* Find the minimal rotation period. */
    idx_t period = L;                     /* default: full length */
    for (idx_t d = 1; d < L; ++d) {
        if (L % d == 0) {                  /* candidate divisor */
            seq_t rot = ((k << d) | (k >> (L - d))) & mask;
            if (rot == k) {               /* period found */
                period = d;
                break;
            }
        }
    }

    /* k is a term iff the period divides k. */
    return (k % (seq_t)period == 0);
}

/* Generate the sequence up to the required index. */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* cached terms */
    static idx_t size      = 0;     /* number of stored terms   */
    static idx_t capacity  = 0;     /* allocated capacity       */

    if (n == 0)                     /* offset is 1; not used */
        return 0;

    /* Ensure we have at least n terms cached. */
    while (size < n) {
        if (size == 0) {             /* first time: seed 0 */
            capacity = 64;
            terms = (seq_t *)malloc(capacity * sizeof(seq_t));
            terms[0] = 0;
            size = 1;
        }

        seq_t k = terms[size - 1] + 1;   /* next candidate */

        /* Find the next valid term. */
        while (!is_term(k))
            ++k;

        /* Append it to the cache. */
        if (size >= capacity) {
            capacity *= 2;
            terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
        }
        terms[size++] = k;
    }

    return terms[n - 1];
}
