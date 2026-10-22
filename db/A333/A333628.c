/*
 * A333628  Runs-resistance of the n-th composition in standard order. Number
 *          of steps taking run-lengths to reduce the n-th composition in
 *          standard order to a singleton.
 * https://oeis.org/A333628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   run_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A333628: runs‑resistance of the n‑th composition in standard order.
   The value a(n) is the number of times the run‑length operation must be
   applied to the n‑th composition (in standard order) before a singleton
   composition is obtained. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/run_lengths.h"

/* Maximum number of parts in any composition that we will handle
   (2^(m‑1) compositions exist for integer m, so m ≤ 64 for any 64‑bit index). */
#define MAX_PARTS 64

/* Reverse the lowest 'bits' bits of x.  The result has the same number
   of bits but in reverse order. */
static uint64_t reverse_bits(uint64_t x, unsigned bits)
{
    uint64_t rev = 0;
    while (bits--) {
        rev <<= 1;
        rev |= x & 1;
        x >>= 1;
    }
    return rev;
}

/* Compute the smallest m such that 2^m – 1 ≥ n+1, i.e.  m = ceil(log2(n+1)).
   n ≥ 1. */
static idx_t ceil_log2_idx(idx_t v)
{
    /* v is guaranteed to be ≥ 1 */
    idx_t msb = 0;
    idx_t temp = v;
    while (temp > 1) {
        temp >>= 1;
        msb++;
    }
    /* msb = floor(log2(v)) */
    if ((idx_t)1 << msb == v)
        return msb;
    else
        return msb + 1;
}

seq_t a(idx_t n)
{
    /* Handle undefined n < 1 safely. */
    if (n == 0)
        return 0;

    /*
     * Determine the integer m for which the n-th composition lies in
     * the list of compositions of m.  The total number of compositions
     * up to integer m is 2^m – 1.  We need the smallest m with
     * 2^m – 1 ≥ n, i.e. 2^m ≥ n+1 → m = ceil(log2(n+1)).
     */
    idx_t m = ceil_log2_idx(n + 1);

    /* Compute the offset within the row of compositions for integer m. */
    idx_t base = (idx_t)1 << (m - 1);                /* 2^(m-1) */
    idx_t offset = n - base;                       /* 0‑based offset */

    /* Construct the composition for this offset. */
    /* n_bits is m‑1, the number of separator positions.  Reverse the
       binary representation of 'offset' with that many bits to obtain
       the separator bits. */
    unsigned n_bits = (unsigned)(m - 1);
    uint64_t rev = reverse_bits(offset, n_bits);

    seq_t parts[MAX_PARTS];
    size_t comp_len = 0;
    seq_t current = 0; /* current part length */

    /* Scan through the elements 1..m, producing parts whenever a
       separator occurs.  Separator after element j corresponds to
       bit (j‑1) of 'rev'. */
    for (unsigned j = 1; j <= m; ++j) {
        current++;                                   /* add this element to the current part */
        if (j <= m - 1 && (rev & ((uint64_t)1 << (j - 1)))) {
            parts[comp_len++] = current;            /* finish current part */
            current = 0;
        }
    }
    parts[comp_len++] = current;                     /* add the final part */

    /* Count how many times we must apply run‑length until a singleton
       composition is obtained. */
    seq_t buffer[MAX_PARTS];
    size_t newlen;
    size_t steps = 0;
    while (comp_len > 1) {
        run_lengths(parts, comp_len, buffer, &newlen);
        /* copy back to parts for the next iteration */
        for (size_t i = 0; i < newlen; ++i)
            parts[i] = buffer[i];
        comp_len = newlen;
        steps++;
    }

    return (seq_t)steps;
}
