/*
 * A352727  Square array A(n, k), n, k >= 0, read by antidiagonals: the binary
 *          expansion of A(n, k) contains the runs of consecutive 1's that
 *          appear both in the binary expansions of n and k.
 * https://oeis.org/A352727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = the value of A(i,j) where (i,j) is the pair of indices
 *            obtained by reading the square array A(n,k) by antidiagonals.
 *            A(n,k) contains the maximal runs of consecutive 1's that appear
 *            in both binary expansions of n and k
 * Range:     u8 n <= 9511, u16 n <= 9511, u32 n <= 9511, u64 n <= 9511,
 *            u128 n <= 9511, big any n
 * Helpers:   antidiagonal_sum, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/antidiagonal_sum.h"

/* Compute A(n,k) as defined in the OEIS entry.  The result type is
   seq_t, but all bitwise logic is performed using idx_t (uint64_t)
   to avoid shift counting past the width of the underlying machine type. */
static seq_t compute_A(idx_t n, idx_t k)
{
    if (n == 0 || k == 0)
        return (seq_t)0;

    const idx_t BITS = (idx_t)sizeof(seq_t) * CHAR_BIT;
    /* bit lengths of n and k; 0 has length 0 */
    idx_t bitlen_n = n == 0 ? 0U : floor_log2_uint64(n) + 1;
    idx_t bitlen_k = k == 0 ? 0U : floor_log2_uint64(k) + 1;

    seq_t result = (seq_t)0;
    idx_t pos = 0;

    /* Traverse runs of 1's in n, but stop when we reach bits beyond
       the width of seq_t because those bits cannot influence the result. */
    while (pos < bitlen_n && pos < BITS) {
        if ((n >> pos) & 1ULL) {        /* start of a run of 1s */
            idx_t start = pos;
            idx_t len   = 0;
            while (pos < bitlen_n && (n >> pos) & 1ULL) {
                len++;
                pos++;
                if (pos >= BITS) break;   /* no further bits can affect result */
            }

            /* Check that the run is maximal in both n and k */
            bool left_ok  = true;
            bool right_ok = true;
            if (start > 0)
                left_ok = !((k >> (start - 1)) & 1ULL);
            if (start + len < bitlen_k)
                right_ok = !((k >> (start + len)) & 1ULL);

            if (left_ok && right_ok) {
                /* Only the part of the run that fits in seq_t can be set. */
                idx_t take = len < (BITS - start) ? len : (BITS - start);
                if (take > 0) {
                    seq_t mask;
                    if (take == BITS) {
                        mask = ~(seq_t)0;   /* all 1's */
                    } else {
                        mask = ((((seq_t)1 << take) - 1) << start);
                    }
                    /* The run contributes only if k has all those bits set. */
                    if (((seq_t)k & mask) == mask)
                        result |= mask;
                }
            }
        } else {
            pos++;   /* skip a 0 */
        }
    }

    return result;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t s = antidiagonal_sum(n);
    idx_t diag_start = (idx_t)(((__int128)s * (s + 1) / 2));
    idx_t offset = n - diag_start;          /* 0 <= offset <= s */
    idx_t i = offset;
    idx_t j = s - offset;
    return compute_A(i, j);
}
