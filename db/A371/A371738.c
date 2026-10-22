/*
 * A371738  Numbers with non-quanimous binary indices. Numbers whose binary
 *          indices have only one set partition with all equal block-sums.
 * https://oeis.org/A371738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   backtrack
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A371738 – numbers with non‑quanimous binary indices.
   A number is valid if its binary indices (positions of 1 bits,
   counting from 1 for the least significant bit) have no partition
   into two or more non‑empty blocks with all blocks having equal
   sums, except the trivial single‑block partition.  The function
   returns the n‑th such number, with a(1)=1. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/backtrack.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;

    for (;;) {
        /* Extract binary‑index positions */
        int pos[64], m = 0;
        uint64_t t = (uint64_t)candidate;
        int bit = 1;
        while (t) {
            if (t & 1)
                pos[m++] = bit;
            t >>= 1;
            ++bit;
        }

        bool ok = true;               /* assume valid until proven otherwise */

        if (m > 2) {
            /* Compute sum of indices and test all possible block counts k */
            int sum = 0;
            for (int i = 0; i < m; ++i) sum += pos[i];

            /* Sort indices in descending order to aid pruning */
            for (int i = 0; i < m; ++i)
                for (int j = i + 1; j < m; ++j)
                    if (pos[i] < pos[j]) {
                        int tmp = pos[i];
                        pos[i] = pos[j];
                        pos[j] = tmp;
                    }

            for (int k = 2; k <= m; ++k) {
                if (sum % k != 0)
                    continue;
                int blockSum = sum / k;
                int blockSums[64] = {0};
                if (backtrack(0, m, k, blockSum, pos, blockSums)) {
                    ok = false;          /* non‑trivial partition found */
                    break;
                }
            }
        }

        if (ok) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
