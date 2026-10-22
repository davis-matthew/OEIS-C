/*
 * A339443  Pairwise listing of the partitions of k into two parts (s,t), with
 *          0 < t <= s ordered by decreasing values of s and where k = 2,3,...
 *          .
 * https://oeis.org/A339443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Computation of A339443: pairwise listing of the partitions of k into two parts (s,t),
   with 0 < t <= s, ordered by decreasing s for each k, concatenated over k = 2,3,...
   The sequence a(n) gives the n‑th integer in that concatenation.
   Implementation uses binary search on cumulative term counts. */

/* Number of terms contributed by all k' <= k in the concatenated sequence.
   S(k) = 2 * floor(k/2) * ceil(k/2), the cumulative count up to integer k. */
static inline __uint128_t terms_upto(idx_t k)
{
    idx_t m = k >> 1;          /* floor(k/2) */
    idx_t q = k - m;           /* ceil(k/2)   */
    return ( (__uint128_t)2 * m * q );   /* 2 * floor(k/2) * ceil(k/2) */
}

seq_t a(idx_t n)
{
    /* Zero‑based target index in the concatenated list. */
    idx_t target = n - 1;

    /* Binary search for the smallest k >= 2 such that terms_upto(k) > target. */
    idx_t low = 2;
    idx_t high = UINT64_MAX;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (terms_upto(mid) > target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t k = low; /* terms from 2..k-1 are <= target < terms from 2..k */

    /* Offset within the block for integer k. */
    __uint128_t prev_terms = terms_upto(k - 1);
    idx_t offset = (idx_t)(target - (idx_t)prev_terms);   /* 0‑based within k */

    /* Determine whether the offset points to an 's' or a 't' value. */
    if ((offset & 1ULL) == 0) {          /* even offset -> 's' */
        idx_t half = offset >> 1;       /* offset/2 */
        return (seq_t)(k - half - 1);
    } else {                            /* odd offset -> 't' */
        return (seq_t)((offset + 1) >> 1);
    }
}
