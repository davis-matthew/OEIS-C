/*
 * A339533  Numbers b > 1 such that the smallest four primes, i.e., 2, 3, 5
 *          and 7 are base-b Wieferich primes.
 * https://oeis.org/A339533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th integer b>1 such that b^(p-1) ≡ 1 (mod p²) for all
 *            p∈{2,3,5,7}
 * Range:     u8 none, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Period is lcm(4,9,25,49) = 44100.  */
    const size_t PERIOD = 44100;
    static bool initialized = false;
    static seq_t residues[48];
    static size_t resid_cnt = 0;

    if (!initialized) {
        bool sol25[25] = {0};
        bool sol49[49] = {0};

        /* solutions modulo 25: b^4 ≡ 1 (mod 25) */
        for (uint8_t x = 1; x < 25; ++x) {
            if (mod_pow((seq_t)x, 4, (seq_t)25) == (seq_t)1)
                sol25[x] = true;
        }

        /* solutions modulo 49: b^6 ≡ 1 (mod 49) */
        for (uint64_t x = 1; x < 49; ++x) {
            if (mod_pow((seq_t)x, 6, (seq_t)49) == (seq_t)1)
                sol49[x] = true;
        }

        /* Collect residues r in [2, PERIOD‑1] that satisfy all conditions */
        for (uint64_t r = 2; r < PERIOD; ++r) {  /* start at 2 to skip r==1 */
            if (r % 4 != 1) continue;

            uint8_t r9 = r % 9;
            if (r9 != 1 && r9 != 8) continue;

            if (!sol25[(size_t)(r % 25)]) continue;
            if (!sol49[(size_t)(r % 49)]) continue;

            residues[resid_cnt++] = r;
            /* in practice resid_cnt will be 47 */
        }
        initialized = true;
    }

    /* Compute the n-th term from the residues */
    size_t idx = n - 1;                  /* 0‑based index */
    size_t cycle_idx = idx / resid_cnt;  /* which period */
    size_t offset_idx = idx % resid_cnt; /* offset within the period */

    return residues[offset_idx] + (seq_t)cycle_idx * (seq_t)PERIOD;
}
