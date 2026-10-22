/*
 * A375478  Irregular triangle read by rows in which row n lists the iterates
 *          of the phi(x) map from n to 1, where phi(x) is Euler's totient
 *          function (A000010).
 * https://oeis.org/A375478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = the nth element of the concatenated lists of iterated
 *            Euler's totient values φ(x) starting from m = 1, 2, 3, ...; each
 *            row m consists of φ^k(m) for k = 0, 1, ..., until 1
 * Range:     u8 n <= 1780, u16 n <= 9523, u32 n <= 9523, u64 n <= 9523,
 *            u128 n <= 9523, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    idx_t cumulative = 0;                // number of terms seen so far
    for (idx_t m = 1; ; ++m) {
        // compute the length of the φ-chain starting at m
        idx_t len = 0;
        seq_t val = (seq_t)m;
        while (1) {
            ++len;
            if (val == (seq_t)1) break;   // chain ends at 1
            val = calc_phi(val);
        }

        if (cumulative + len >= n) {
            // the desired term lies in row m
            idx_t offset = n - cumulative - 1;   // 0‑based position inside the row
            val = (seq_t)m;
            for (idx_t i = 0; i < offset; ++i)
                val = calc_phi(val);
            return val;
        }

        cumulative += len;   // skip this whole row and continue
    }
}
