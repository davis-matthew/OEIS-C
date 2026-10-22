/*
 * A380036  Smallest number which is not a triangular number mod n.
 * https://oeis.org/A380036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest nonnegative integer not congruent to
 *            T(k)=k(k+1)/2 mod n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = n;                     /* assume all residues are seen */
    idx_t const limit = 2 * n;            /* enough to cover all possible residues */

    unsigned char *seen = (unsigned char *)malloc((size_t)n);
    if (!seen)
        return result;                  /* fallback if allocation fails */

    memset(seen, 0, (size_t)n);
    size_t seen_cnt = 0;
    seq_t cur  = 0;                       /* current triangular residue */
    seq_t add  = 1 % n;                   /* first increment (k+1) mod n */
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 0; k < limit; ++k) {
        if (!seen[(size_t)cur]) {
            seen[(size_t)cur] = 1;
            ++seen_cnt;
            if (seen_cnt == n) {          /* all residues visited */
                result = n;
                break;
            }
        }
        cur = (cur + add) % n_seq;        /* move to next triangular residue */
        ++add;
        if (add >= n_seq)
            add -= n_seq;                 /* keep add in [0, n-1] */
    }

    if (seen_cnt < n) {                /* find the smallest missing residue */
        for (idx_t t = 0; t < n; ++t) {
            if (!seen[(size_t)t]) {
                result = t;
                break;
            }
        }
    }

    free(seen);
    return result;
}
