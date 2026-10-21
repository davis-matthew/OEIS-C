/*
 * A289071  Fixed point of the mapping 00->0010, 01->010, 10->100, starting
 *          with 00.
 * https://oeis.org/A289071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A289071: fixed point of the mapping
   00 → 0010, 01 → 010, 10 → 100, starting with 00.
   a(n) is the n‑th bit (1‑based) of the infinite word
   obtained by iteratively replacing non‑overlapping
   consecutive pairs according to the rule above.
   The algorithm builds the word until it contains the
   requested index.  It works for all n ≥ 1. */

#include <stdlib.h>

static seq_t *seq = NULL;
static size_t seq_len = 0;

/* Expand the built word so that its length is at least `target`. */
static void
ensure_len(idx_t target)
{
    size_t t = (size_t)target;
    if (seq_len >= t)
        return;

    /* Initialise with the starting word "00". */
    if (seq_len == 0)
    {
        seq_len = 2;
        seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
        seq[0] = 0;
        seq[1] = 0;
    }

    /* Repeatedly apply the morphism until the word is long enough. */
    while (seq_len < t)
    {
        size_t cur_len = seq_len;
        size_t new_len = 0;

        /* Compute new length: each pair expands to 3 or 4 bits,
           plus a possible leftover single bit. */
        for (size_t i = 0; i + 1 < cur_len; i += 2)
        {
            seq_t a = seq[i];
            seq_t b = seq[i + 1];
            if (a == 0 && b == 0)
                new_len += 4;          /* 0010 */
            else
                new_len += 3;          /* 010 or 100 */
        }
        new_len += (cur_len & 1);      /* leftover single bit */

        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i + 1 < cur_len; i += 2)
        {
            seq_t a = seq[i];
            seq_t b = seq[i + 1];
            if (a == 0 && b == 0)
            {
                new_buf[pos++] = 0;
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
            }
            else if (a == 0 && b == 1)   /* 01 → 010 */
            {
                new_buf[pos++] = 0;
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
            }
            else                         /* 10 → 100 */
            {
                new_buf[pos++] = 1;
                new_buf[pos++] = 0;
                new_buf[pos++] = 0;
            }
        }
        if (cur_len & 1)                    /* copy leftover bit */
            new_buf[pos++] = seq[cur_len - 1];

        free(seq);
        seq = new_buf;
        seq_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0, but not used */
    ensure_len(n);
    return seq[(size_t)n - 1];
}
