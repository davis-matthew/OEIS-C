/*
 * A317510  Numbers (4p+1)/3 where p is a Sophie Germain prime p > 3.
 * https://oeis.org/A317510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4p+1)/3 where p is the nth Sophie Germain prime > 3 (the
 *            sequence starts with a(1) = 7 for p = 5)
 * Range:     u8 n <= 13, u16 n <= 657, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */
#include "../utils/ensure_capacity.h"  /* void ensure_capacity(size_t, size_t*, void**, size_t); */

static seq_t *sg_vals = NULL;        /* Array of sequence values */
static size_t sg_count = 0;          /* Number of values stored */
static size_t sg_capacity = 0;
static idx_t next_candidate = 5;     /* Next odd number to test (first SG prime > 3) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined but safe */

    /* Expand_sequence until we have at least n terms */
    while (sg_count < n) {
        /* Ensure we have space */
        ensure_capacity(sg_count + 1, &sg_capacity, (void **)&sg_vals,
                        sizeof(seq_t));

        /* Find next Sophie Germain prime */
        idx_t p = next_candidate;
        while (1) {
            if (is_prime(p) && is_prime(p * 2 + 1)) {
                /* Compute (4p+1)/3 as seq_t */
                seq_t val = ((seq_t)4 * p + 1) / 3;
                sg_vals[sg_count++] = val;
                next_candidate = p + 2;   /* Next odd number after p */
                break;
            }
            p += 2;   /* Only test odd numbers */
        }
    }

    return sg_vals[n - 1];   /* 1-based indexing */
}
