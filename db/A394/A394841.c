/*
 * A394841  A variant of Wallis sieve, read by antidiagonals.
 * https://oeis.org/A394841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the (i,j) cell read by antidiagonal enumeration is
 *            not a central square at any level of the recursive construction;
 *            a(n) = 0 otherwise. This implements OEIS A394841
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 8368 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Convert n into coordinates (i,j) using antidiagonal order. */
static inline void _coords_from_antidiagonal(idx_t n, idx_t *ii, idx_t *jj)
{
    /* Find the smallest diagonal k such that
       T_k = (k+1)(k+2)/2 > n. */
    idx_t k = 0;
    idx_t T = 1;               /* T_0 = 1 */
    while (T <= n)
    {
        ++k;
        T += k + 1;             /* add length of next diagonal */
    }
    idx_t prev_T = T - (k + 1);/* sum of lengths before diagonal k */
    idx_t offset = n - prev_T;
    *ii = offset;              /* row coordinate */
    *jj = k - offset;          /* column coordinate */
}

/* Return the value (0 or 1) at coordinates (i,j) in the infinite matrix. */
static inline seq_t _value_at(idx_t i, idx_t j)
{
    /* Base case: the single cell of W_0. */
    if (i == 0 && j == 0)
        return (seq_t)1;

    /* Determine the maximum coordinate to know how many levels are needed. */
    idx_t maxc = (i > j) ? i : j;

    /* Pre‑compute the odd double‑factorial block sizes S_k = 1*3*5*...*(2k+1). */
    idx_t sizes[64];
    sizes[0] = 1;
    idx_t level = 1;
    while (sizes[level - 1] <= maxc)
    {
        /* S_{level} = S_{level-1} * (2*level+1) */
        idx_t next = sizes[level - 1] * (2 * level + 1);
        if (next > maxc)      /* no larger block needed */
            break;
        sizes[level] = next;
        ++level;
    }

    /* Start from the largest block that still covers the point. */
    idx_t cur_level = level;
    idx_t block_size = sizes[level - 1];   /* size of each child block */

    while (true)
    {
        /* Which child block (in the (2*cur_level+1) grid) contains the point? */
        idx_t cell_i = i / block_size;
        idx_t cell_j = j / block_size;

        /* The central block corresponds to indices equal to cur_level. */
        if (cell_i == cur_level && cell_j == cur_level)
            return (seq_t)0;   /* cell lies in a zero block */

        /* Move into the chosen child block. */
        i -= cell_i * block_size;
        j -= cell_j * block_size;

        if (cur_level == 1)  /* reached the base 1×1 block */
            return (seq_t)1;

        /* Descend one level. */
        --cur_level;
        block_size = sizes[cur_level - 1];
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t i, j;
    _coords_from_antidiagonal(n, &i, &j);
    return _value_at(i, j);
}
