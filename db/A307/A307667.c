/*
 * A307667  Lexicographically earliest sequence of positive terms, such that
 *          each value, say v, appears four times at indices k, k + v, k + 2*v
 *          and k + 3*v for some k.
 * https://oeis.org/A307667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   lexicographically earliest sequence with each value v appearing
 *            at indices k, k+v, k+2v, k+3v. This implementation uses a greedy
 *            construction and memoises results
 * Range:     u8 n <= 714, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static state ----------------------------------------------------------- */
static seq_t *seq = NULL;      /* sequence values, seq[0] unused      */
static size_t seq_cap = 0;     /* current allocated size of seq      */
static idx_t next_index = 1;   /* next index that needs to be filled */

static uint8_t *cnt = NULL;    /* how many times a value has been used  */
static size_t cnt_cap = 0;     /* current allocated size of cnt         */

/* Grow the seq array to at least 'need' elements (indices 0..need-1). */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) /* allocation failure */
        return;
    for (size_t i = seq_cap; i < new_cap; ++i)
        new_seq[i] = 0;
    seq = new_seq;
    seq_cap = new_cap;
}

/* Grow the cnt array to at least 'need' elements (values 0..need-1). */
static void ensure_cnt_cap(size_t need)
{
    if (need <= cnt_cap)
        return;
    size_t new_cap = cnt_cap ? cnt_cap * 2 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    uint8_t *new_cnt = (uint8_t *)realloc(cnt, new_cap * sizeof(uint8_t));
    if (!new_cnt) /* allocation failure */
        return;
    for (size_t i = cnt_cap; i < new_cap; ++i)
        new_cnt[i] = 0;
    cnt = new_cnt;
    cnt_cap = new_cap;
}

/* ----------------------------------------------------------------------- */
/* Return the n-th term of the sequence (1‑based index).                    */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for indices < 1 */

    /* Ensure capacity for the requested index. */
    ensure_seq_cap((size_t)n + 1);

    /* Generate sequence entries up to n if necessary. */
    while (next_index <= n) {
        /* Skip already filled slots. */
        if (seq[(size_t)next_index] != 0) {
            ++next_index;
            continue;
        }

        /* Find the smallest value v that can be legally placed at this index. */
        for (seq_t v = 1; ; ++v) {
            size_t iv = (size_t)v;
            ensure_cnt_cap(iv + 1);

            /* Skip values that are already fully used (4 placements). */
            if (cnt[iv] >= 4)
                continue;

            /* Compute the four target indices.  v is cast to idx_t for the addition. */
            idx_t i1 = next_index;
            idx_t i2 = i1 + (idx_t)v;
            idx_t i3 = i2 + (idx_t)v;
            idx_t i4 = i3 + (idx_t)v;

            /* Ensure the array can hold the farthest required index. */
            ensure_seq_cap((size_t)i4 + 1);

            /* Place v only if all four positions are currently empty. */
            if (seq[(size_t)i1] == 0 && seq[(size_t)i2] == 0 &&
                seq[(size_t)i3] == 0 && seq[(size_t)i4] == 0) {

                seq[(size_t)i1] = seq[(size_t)i2] =
                seq[(size_t)i3] = seq[(size_t)i4] = v;
                cnt[iv] += 4;
                break;
            }
        }
        ++next_index;
    }

    return seq[(size_t)n];
}
