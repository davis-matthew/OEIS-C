/*
 * A337695  Numbers k such that the distinct parts of the k-th composition in
 *          standard order (A066099) are not pairwise coprime, where a
 *          singleton is always considered coprime.
 * https://oeis.org/A337695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th k such that the distinct parts of the k‑th
 *            composition in standard order (A066099) are not pairwise
 *            coprime. Computed by brute‑force scanning from k = 1 upwards
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety, though n ≥ 1 by definition */

    idx_t found = 0;  /* number of k's satisfying the condition found so far */
    idx_t k = 1;      /* current composition index */

    while (1) {
        /* --- generate the k‑th composition via the binary positions method --- */
        uint64_t kk = k;
        uint64_t part_arr[64];
        size_t pcnt = 0;
        uint64_t prev = 0;
        uint64_t i = 1;  /* bit position counter (LSB = 1) */

        while (kk) {
            if (kk & 1ULL) {
                part_arr[pcnt++] = i - prev;  /* difference of positions */
                prev = i;
            }
            kk >>= 1;
            ++i;
        }

        /* reverse the parts to get the composition in standard order */
        for (size_t l = 0, r = pcnt - 1; l < r; ++l, --r) {
            uint64_t tmp = part_arr[l];
            part_arr[l] = part_arr[r];
            part_arr[r] = tmp;
        }

        /* --- collect distinct parts --- */
        seq_t distinct[64];
        size_t dcnt = 0;
        for (size_t p = 0; p < pcnt; ++p) {
            seq_t val = (seq_t)part_arr[p];
            bool seen = false;
            for (size_t q = 0; q < dcnt; ++q) {
                if (distinct[q] == val) { seen = true; break; }
            }
            if (!seen) distinct[dcnt++] = val;
        }

        /* --- test the “not pairwise coprime” condition --- */
        if (dcnt > 1) {
            bool bad = false;
            for (size_t a = 0; a < dcnt && !bad; ++a) {
                for (size_t b = a + 1; b < dcnt; ++b) {
                    seq_t g = gcd(distinct[a], distinct[b]);
                    if (g != (seq_t)1) { bad = true; break; }
                }
            }
            if (bad) {
                ++found;
                if (found == n) return (seq_t)k;
            }
        }

        ++k;  /* move to the next composition */
    }
}
