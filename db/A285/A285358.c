/*
 * A285358  Fixed point of the morphism 0 -> 10, 1 -> 1101.
 * https://oeis.org/A285358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = value of the n‑th bit (1‑based) in the fixed point of the
 *            morphism 0 → 10, 1 → 1101, starting from the seed symbol 1. The
 *            sequence is the infinite word obtained by repeatedly applying
 *            the morphism to the symbol 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached word produced by successive applications of the morphism. */
static seq_t *bits = NULL;
static idx_t bits_len = 0;

/* Expand the current word once using the morphism rules. */
static void expand(void)
{
    idx_t old_len = bits_len;
    size_t new_len = 0;

    for (idx_t i = 0; i < old_len; ++i)
        new_len += (bits[i] == 0) ? 2U : 4U;

    seq_t *nw = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!nw) exit(EXIT_FAILURE);

    idx_t pos = 0;
    for (idx_t i = 0; i < old_len; ++i) {
        if (bits[i] == 0) {
            nw[pos++] = (seq_t)1;
            nw[pos++] = (seq_t)0;
        } else {
            nw[pos++] = (seq_t)1;
            nw[pos++] = (seq_t)1;
            nw[pos++] = (seq_t)0;
            nw[pos++] = (seq_t)1;
        }
    }

    free(bits);
    bits = nw;
    bits_len = (idx_t)new_len;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, n==0 undefined */

    if (bits_len == 0) {                  /* initialise with the seed 1 */
        bits = (seq_t *)malloc(sizeof(seq_t));
        if (!bits) exit(EXIT_FAILURE);
        bits[0] = (seq_t)1;
        bits_len = 1;
    }

    /* Grow the word until it contains at least n bits. */
    while (bits_len < n)
        expand();

    return bits[n - 1];
}
