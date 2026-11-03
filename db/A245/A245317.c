/*
 * A245317  Concatenate n-th composite integer with n.
 * https://oeis.org/A245317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_parts(c_n, n) where c_n is the n-th composite
 *            integer
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9065 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1, but return 0 for safety */

    /* static cache of composites */
    static seq_t *comp_arr = NULL;   /* array of composites (seq_t) */
    static size_t comp_len = 0;      /* number of composites cached */
    static size_t comp_cap = 0;      /* allocated capacity */
    static idx_t next = 4;           /* next integer to test for compositeness */

    /* Expand cache until we have at least n composites */
    while (comp_len < (size_t)n)
    {
        /* test if 'next' is composite */
        if (!is_prime_seq((uint64_t)next))
        {
            /* add to cache */
            append_candidate(&comp_arr, &comp_len, &comp_cap, (seq_t)next);
        }
        ++next;
    }

    /* Return concatenation of nth composite with n */
    return concat_parts((idx_t)comp_arr[n - 1], n);
}
