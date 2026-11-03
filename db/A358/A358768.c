/*
 * A358768  Numbers k with an odd number of prime factors (when counted with
 *          multiplicity) in sigma(k), the sum of divisors of k.
 * https://oeis.org/A358768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 9680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9681 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k where σ(k) has an odd number of prime factors (counted with multiplicity). */
/* Computation is by brute‑force enumeration of k, using helper routines for σ and factor counting. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Memoization state */
    static seq_t *seq = NULL;   /* stored k values in seq_t */
    static size_t seq_len = 0;  /* how many have been found */
    static size_t seq_cap = 0;  /* allocated capacity */
    static idx_t current_k = 1; /* last k that has been examined */

    /* Produce terms up to index n */
    while (seq_len < (size_t)n)
    {
        ++current_k; /* next candidate k */

        /* Compute σ(k) as a seq_t value */
        seq_t sig = sigma_of(current_k);

        /* Count prime factors of σ(k) with multiplicity */
        seq_t omega = bigomega_seq(sig);

        /* Keep k if Ω(σ(k)) is odd */
        if ((omega % 2) == 1)
        {
            /* Ensure space in the sequence array */
            if (seq_len + 1 > seq_cap)
                ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq, sizeof(seq_t));

            seq[seq_len++] = (seq_t)current_k;
        }
    }

    /* Return the (n-1)th stored value (offset 1) */
    return seq[seq_len - 1];
}
