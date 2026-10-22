/*
 * A354896  A fixed point of the two-block substitution 00->001, 01->010,
 *          10->101, 11->110.
 * https://oeis.org/A354896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth bit of the fixed point of the two‑block
 *            substitution 00 → 001, 01 → 010, 10 → 101, 11 → 110 starting
 *            with 00, i.e. the word obtained by iterating the map κ' defined
 *            in the OEIS entry
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Mapping table for the 2‑bit blocks */
static const char kappa_map[4][4] = {
    "001", /* 00 */
    "010", /* 01 */
    "101", /* 10 */
    "110"  /* 11 */
};

/* Apply one step of κ' to the word s of length len.
   The result is stored in a newly allocated buffer whose size is
   written into *new_len. The caller must free the returned buffer. */
static char *kappa_prime(const char *s, size_t len, size_t *new_len)
{
    /* If len is odd, ignore the last bit temporarily. */
    size_t even_len = len & ~((size_t)1);
    *new_len = 3 * (even_len / 2);
    char *out = (char *)malloc(*new_len);
    size_t pos = 0;
    for (size_t i = 0; i < even_len; i += 2) {
        unsigned idx = ((unsigned)(s[i] - '0') << 1) | (s[i + 1] - '0');
        const char *pat = kappa_map[idx];
        out[pos++] = pat[0];
        out[pos++] = pat[1];
        out[pos++] = pat[2];
    }
    return out;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* sequence is 1‑based; behaviour below 1 is irrelevant */

    /* Initial word "0011" (the fixed point starts with these 4 bits). */
    size_t len = 4;
    char *w = (char *)malloc(len);
    w[0] = '0'; w[1] = '0'; w[2] = '1'; w[3] = '1';

    /* Grow the word until it covers the requested index. */
    while (len < (size_t)n) {
        size_t new_len;
        char *nw = kappa_prime(w, len, &new_len);
        free(w);
        w = nw;
        len = new_len;
    }

    /* Retrieve the n‑th bit (0‑based index). */
    unsigned bit = (unsigned)(w[n - 1] - '0');

    free(w);
    return (seq_t)bit;
}
