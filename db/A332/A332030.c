/*
 * A332030  a(n) is the product of the distinct positive numbers whose binary
 *          digits appear in order, but not necessarily as consecutive digits,
 *          in the binary representation of n.
 * https://oeis.org/A332030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the product of the distinct positive integers whose *
 *            binary digits appear in order (not necessarily consecutively) in
 *            the * binary representation of n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 52, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on uint64_t. */
static int cmp_u64(const void *p, const void *q) {
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    return (*a > *b) - (*a < *b);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* a(0) = 1 by definition. */
    if (n == 0ULL) return (seq_t)1;

    /* Determine bit length of n. */
    idx_t tmp = n;
    size_t bitlen = 0;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }

    /* Enumerate all non‑empty subsets of the bit positions. */
    size_t total_masks = ((size_t)1 << bitlen);
    uint64_t *vals = (uint64_t *)malloc(sizeof(uint64_t) * (total_masks - 1));
    size_t count = 0;

    for (size_t mask = 1; mask < total_masks; ++mask) {
        /* Find the most significant selected bit. */
        int first_selected = -1;   /* -1: none, -2: leading zero (invalid)  */
        for (size_t i = 0; i < bitlen; ++i) {
            uint64_t bit = (n >> (bitlen - 1 - i)) & 1ULL;
            if (mask & ((size_t)1 << (bitlen - 1 - i))) {
                if (bit == 0) {
                    first_selected = -2;   /* leading zero -> discard */
                    break;
                } else {
                    first_selected = (int)i;
                    break;
                }
            }
        }
        if (first_selected == -2) continue;

        /* Build the subsequence value. */
        uint64_t cur = 0;
        for (size_t i = 0; i < bitlen; ++i) {
            uint64_t bit = (n >> (bitlen - 1 - i)) & 1ULL;
            if (mask & ((size_t)1 << (bitlen - 1 - i))) {
                cur = (cur << 1) | bit;          /* cur = cur*2 + bit */
            }
        }
        vals[count++] = cur;
    }

    if (count == 0) {            /* Should not happen for n > 0. */
        free(vals);
        return (seq_t)1;
    }

    /* Sort and deduplicate the uint64_t values. */
    qsort(vals, count, sizeof(uint64_t), cmp_u64);

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        if (i == 0 || vals[i] != vals[i - 1]) {
            prod *= (seq_t)vals[i];
        }
    }

    free(vals);
    return prod;
}
