/*
 * A343174  Partial sums of paper-folding sequence A014577.
 * https://oeis.org/A343174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004526(n+2) + A033264(n+1)
 * Range:     u8 n <= 507, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *partial = NULL;   /* partial sums of the paper‑folding sequence */
    static size_t seq_len = 0;      /* number of pre‑computed terms (length of partial) */

    if ((size_t)n < seq_len) {
        return partial[n];
    }

    /* Compute the minimal length 2^k‑1 that covers indices 0…n */
    size_t target = (size_t)n + 1;
    size_t len = 1;
    while (len < target)  /* keep doubling until len ≥ target */
        len = 2 * len + 1;

    /* Generate the paper‑folding bits up to length len */
    uint8_t *bits = (uint8_t*)malloc(len * sizeof(uint8_t));
    bits[0] = 1;               /* s(0) = 1 */
    size_t cur = 1;             /* current length of bits */

    while (cur < len) {
        size_t next = 2 * cur + 1;
        uint8_t *new_bits = (uint8_t*)malloc(next * sizeof(uint8_t));

        for (size_t i = 0; i < cur; ++i) {
            new_bits[i] = bits[i];
        }
        new_bits[cur] = 1; /* middle element */

        for (size_t i = 0; i < cur; ++i) {
            /* complement and reverse the old bits */
            new_bits[cur + 1 + i] = 1 - bits[cur - 1 - i];
        }

        free(bits);
        bits = new_bits;
        cur  = next;
    }

    /* Compute partial sums */
    seq_t *new_cache = (seq_t*)malloc(len * sizeof(seq_t));
    new_cache[0] = bits[0];
    for (size_t i = 1; i < len; ++i) {
        new_cache[i] = new_cache[i - 1] + bits[i];
    }

    free(bits);
    free(partial);
    partial = new_cache;
    seq_len = len;

    return partial[n];
}
