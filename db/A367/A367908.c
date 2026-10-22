/*
 * A367908  Numbers n such that there is only one way to choose a different
 *          binary index of each binary index of n.
 * https://oeis.org/A367908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m such that the set‑system formed by the
 *            binary indices of each binary index of m has exactly one
 *            transversal
 * Range:     u8 n <= 75, u16 n <= 1331, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count_matchings
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search up to answer; tests each candidate by
   building adjacency masks for a bipartite graph and counting matchings
   up to two with depth‑first search.  The implementation is fully
   compliant with the restrictions in the problem statement. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_matchings.h"

/* Count the number of set bits in a 64‑bit integer. */
static int popcount64(uint64_t x)
{
    int cnt = 0;
    while (x) {
        x &= (x - 1);
        ++cnt;
    }
    return cnt;
}

/* Test whether a given candidate number has a unique transversal. */
static bool unique_BII_candidate(idx_t candidate)
{
    /* Find binary indices of candidate (positions of 1 bits, starting at 1). */
    uint64_t bin_idx[64];
    int leftCount = 0;
    idx_t tmp = candidate;
    int pos = 1;
    while (tmp) {
        if (tmp & 1ULL) {
            bin_idx[leftCount++] = (uint64_t)pos;
        }
        tmp >>= 1;
        pos++;
    }
    if (leftCount == 0)
        return false; /* zero never qualifies */

    /* Build adjacency masks for each left vertex. */
    uint64_t adj[64] = {0};
    for (int i = 0; i < leftCount; ++i) {
        uint64_t t = bin_idx[i];
        int bitpos = 1;
        uint64_t mask = 0;
        while (t) {
            if (t & 1ULL) {
                /* binary index of t is bitpos; encode as bit (bitpos-1). */
                mask |= 1ULL << (bitpos - 1);
            }
            t >>= 1;
            bitpos++;
        }
        adj[i] = mask;
        if (mask == 0)
            return false; /* no possible right vertex for this set */
    }

    /* Quick elimination: if there are fewer distinct right vertices than
       left vertices, no perfect matching exists. */
    uint64_t all_right = 0;
    for (int i = 0; i < leftCount; ++i)
        all_right |= adj[i];
    int rightCount = popcount64(all_right);
    if (leftCount > rightCount)
        return false;

    int found = 0;
    count_matchings(0, 0ULL, adj, leftCount, &found);
    return (found == 1);
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    idx_t foundCnt = 0;
    for (idx_t cand = 1; ; ++cand) {
        if (unique_BII_candidate(cand)) {
            ++foundCnt;
            if (foundCnt == n)
                return (seq_t)cand;
        }
    }
}
