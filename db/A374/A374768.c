/*
 * A374768  Numbers k such that the leaders of weakly increasing runs in the
 *          k-th composition in standard order (A066099) are distinct.
 * https://oeis.org/A374768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest nonnegative integer k such that the
 *            leaders of the weakly increasing runs in the k‑th composition in
 *            standard order (A066099) are distinct
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a given non‑negative integer k satisfies the property. */
static bool good_k(idx_t k)
{
    /* Compute the parts of the composition of k defined by A066099. */
    int parts[65];
    int mp = 0;          /* number of parts obtained before reversal   */
    int prev = 0;        /* previous position in reversed binary (1‑based) */
    idx_t tmp = k;
    int pos = 1;         /* current bit position in reversed binary (1‑based) */

    while (tmp) {
        if (tmp & 1ULL) {
            parts[mp++] = pos - prev;  /* difference gives a part size   */
            prev = pos;
        }
        tmp >>= 1;
        pos++;
    }

    /* For k = 0 the composition is empty – it trivially satisfies the property. */
    if (mp == 0)
        return true;

    /* Build the composition array as the reverse of the obtained parts. */
    int comp[65];
    for (int i = 0; i < mp; ++i)
        comp[i] = parts[mp - 1 - i];

    /* Walk through the composition to obtain leaders of weakly increasing runs,
       checking that all leaders are distinct. */
    int leaders[65];
    int lcnt = 0;
    int i = 0;
    while (i < mp) {
        int leader = comp[i];

        for (int t = 0; t < lcnt; ++t)
            if (leaders[t] == leader)
                return false;

        leaders[lcnt++] = leader;

        int j = i;
        while (j + 1 < mp && comp[j + 1] >= comp[j])
            ++j;
        i = j + 1;
    }

    return true;
}

/* Return the n-th term of the sequence A374768. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next_k = 0;   /* next non‑negative integer to test */

    /* Generate sequence terms until we have at least n of them. */
    while (seq_len < n) {
        if (good_k(next_k)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return seq[n - 1];
}
