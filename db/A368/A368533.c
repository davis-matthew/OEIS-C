/*
 * A368533  Numbers whose binary indices are all squarefree.
 * https://oeis.org/A368533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   this sequence consists of the numbers whose set bit positions
 *            (counted from 1) are all squarefree. a(n) is the nth smallest
 *            such number (1‑based)
 * Range:     u8 n <= 64, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* precompute which bit positions 1..64 are squarefree */
    static bool squarefree[65] = {0};
    static bool ready = false;
    if (!ready) {
        for (uint64_t i = 1; i < 65; ++i) {
            bool ok = true;
            for (uint64_t d = 2; d * d <= i; ++d) {
                if (i % (d * d) == 0) {
                    ok = false;
                    break;
                }
            }
            squarefree[i] = ok;
        }
        ready = true;
    }

    uint64_t count = 0;
    unsigned long long cand = 0;

    while (true) {
        /* check if all set-bit positions are squarefree */
        bool ok = true;
        unsigned long long temp = cand;
        uint64_t pos = 1;
        while (temp) {
            if (temp & 1ULL) {
                if (!squarefree[pos]) {
                    ok = false;
                    break;
                }
            }
            temp >>= 1;
            ++pos;
        }

        if (ok) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
        ++cand;
    }
}
