/*
 * A246857  Numbers k such that sigma(k + sigma(k)) = 2*sigma(k).
 * https://oeis.org/A246857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A246857: numbers k such that σ(k + σ(k)) = 2·σ(k)
   via brute‑force search. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

static seq_t *seq = NULL;    /* stored sequence values */
static size_t seq_len = 0;   /* how many values are stored */
static size_t seq_cap = 0;   /* allocated capacity of seq[] */
static idx_t next_k = 2;     /* next integer to test */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)  /* offset 1 – undefined for n = 0 */
        return (seq_t)0;

    /* Initialise storage on first call. */
    if (!seq) {
        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;  /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* compute σ(next_k) */
        seq_t sigma_k = sigma_of(next_k);

        /* compute next_k + σ(next_k) – we assume the sum fits in idx_t */
        idx_t sum_idx = next_k + (idx_t)sigma_k;

        /* compute σ(next_k + σ(next_k)) */
        seq_t sigma_sum = sigma_of(sum_idx);

        /* check the defining condition */
        if (sigma_sum == sigma_k * ((seq_t)2)) {
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                if (!seq) return (seq_t)0;  /* allocation failure */
            }
            seq[seq_len++] = next_k;
        }
        ++next_k;
    }

    return seq[(size_t)n - 1];
}
