/*
 * A372651  a(n) is the product of the distinct nonzero quadratic residues of
 *          n.
 * https://oeis.org/A372651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of distinct non-zero quadratic residues of n
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 21, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1) return 1;

    /* Keep a reusable bitset of size at least n */
    static unsigned char *seen = NULL;
    static idx_t seen_cap = 0;

    /* Extend the bitset if needed */
    if (n > seen_cap) {
        free(seen);
        /* C++ needs a cast of the result of calloc */
        seen = (unsigned char*)calloc((size_t)n, sizeof(unsigned char));
        seen_cap = n;
        if (seen == NULL) return 0;   /* fallback on allocation failure */
    } else {
        memset(seen, 0, (size_t)n * sizeof(unsigned char));
    }

    seq_t prod = 1;
    for (idx_t a_val = 1; a_val < n; ++a_val) {
        /* Compute the residue a_val^2 mod n */
        seq_t r = ((seq_t)a_val * (seq_t)a_val) % (seq_t)n;
        if (r == 0) continue;                /* skip zero residue */
        size_t idx = (size_t)r;              /* r < n, safe to cast */
        if (!seen[idx]) {
            seen[idx] = 1;
            prod *= r;                       /* multiply the distinct residue */
        }
    }
    return prod;
}
