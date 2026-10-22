/*
 * A363284  Numbers that are square or square pyramidal.
 * https://oeis.org/A363284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sorted union of {k^2 : k ≥ 0} ∪ {k(k+1)(2k+1)/6 : k ≥ 1}
 * Range:     u8 n <= 22, u16 n <= 310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static state for lazy generation of the sequence */
static seq_t *seq_arr = NULL;   /* array of generated terms          */
static size_t seq_sz  = 0;      /* number of terms generated so far   */
static size_t seq_cap = 0;      /* current allocation capacity       */

static idx_t  sq_i  = 0;            /* current index for squares    */
static idx_t  pyr_i = 1;            /* current index for pyramids   */
static seq_t  next_sq   = 0;        /* next square value   (sq_i^2) */
static seq_t  next_pyr  = 1;        /* next pyramid value (pyr_i(pyr_i+1)(2pyr_i+1)/6) */

/* Ensure that seq_arr has capacity for at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Generate terms up to at least 'n' terms (1‑based). */
static void
generate_up_to(idx_t n)
{
    while ((idx_t)seq_sz < n)
    {
        /* Ensure we have room for the new element before writing. */
        ensure_capacity(seq_sz + 1);

        seq_t val;
        if (next_sq < next_pyr)
        {   /* next square is smaller */
            val = next_sq;
            ++sq_i;
            seq_t m = (seq_t)sq_i;
            next_sq = m * m;
        }
        else if (next_pyr < next_sq)
        {   /* next pyramid is smaller */
            val = next_pyr;
            ++pyr_i;
            seq_t p = (seq_t)pyr_i;
            next_pyr = p * (p + 1) * (2 * p + 1) / 6;
        }
        else
        {   /* equal: insert once and advance both */
            val = next_sq;
            ++sq_i;
            seq_t m = (seq_t)sq_i;
            next_sq = m * m;
            ++pyr_i;
            seq_t p = (seq_t)pyr_i;
            next_pyr = p * (p + 1) * (2 * p + 1) / 6;
        }

        seq_arr[seq_sz++] = val;
    }
}

/* Public function: return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined by OEIS offset; return 0 for safety */
        return 0;
    if ((idx_t)seq_sz < n)
        generate_up_to(n);
    return seq_arr[n - 1];
}
