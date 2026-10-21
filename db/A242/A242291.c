/*
 * A242291  Greedy-summable tetrahedral numbers.
 * https://oeis.org/A242291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth greedy‑summable tetrahedral number. Let t(k) =
 *            k(k+1)(k+2)/6 be the k‑th tetrahedral number. A tetrahedral
 *            number t(k) is greedy‑summable if it can be written as a sum of
 *            distinct smaller tetrahedral numbers using the greedy algorithm
 *            (always subtract the largest unused tetrahedral num...
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 889, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq = NULL;      /* already found a(n) values */
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t *tetra = NULL;    /* list of all tetrahedral numbers seen */
    static size_t tetra_len = 0, tetra_cap = 0;

    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    /* generate terms until we have enough */
    while (seq_len < (size_t)n)
    {
        /* next tetrahedral index k (1‑based) */
        size_t k = tetra_len + 1;

        /* compute t(k) = k(k+1)(k+2)/6 */
        seq_t kk = (seq_t)k;
        seq_t t = kk * (kk + 1) * (kk + 2) / 6;

        /* enlarge tetra array if needed */
        if (tetra_len == tetra_cap)
        {
            size_t new_cap = tetra_cap ? tetra_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(tetra, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* out of memory */
            tetra = tmp;
            tetra_cap = new_cap;
        }
        tetra[tetra_len] = t;
        tetra_len++;

        /* check greedy‑summability of t(k) (only if k≥2) */
        seq_t rem = t;
        if (k >= 2)
        {
            int idx = (int)(k - 2);     /* start with t(k-1) */
            while (idx >= 0)
            {
                seq_t v = tetra[idx];
                if (v <= rem)
                {
                    rem -= v;
                    if (rem == 0)
                        break;
                }
                idx--;
            }
        }

        if (rem == 0)      /* greedy‑summable */
        {
            if (seq_len == seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = t;
        }
    }

    return seq[(size_t)n - 1];
}
