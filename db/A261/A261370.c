/*
 * A261370  Permutation of nonnegative integers where a number having digits
 *          in nondescending order is followed by all numbers having the same
 *          digits arranged in increasing order.
 * https://oeis.org/A261370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is obtained by enumerating each permutation class of *
 *            decimal digits in numeric order: for every integer whose digits
 *            are * sorted in non‑decreasing order (with no leading zero)
 *            output that * canonical integer, then output all other
 *            permutations of the same * multiset that do not start with...
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_permutation_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_permutation_arr.h"

static seq_t digits_to_value(const uint8_t *d, int len) {
    seq_t val = 0;
    for (int i = 0; i < len; ++i) {
        val = (val * (seq_t)10) + d[i];
    }
    return val;
}

/* Public sequence function */
seq_t a(idx_t n) {
    idx_t produced = 0;
    seq_t current = 0;
    while (1) {
        /* count digits of current */
        uint8_t cnt0[10] = {0};
        seq_t tmp = current;
        if (tmp == (seq_t)0) {
            cnt0[0] = 1;
        } else {
            while (tmp > (seq_t)0) {
                uint8_t d = (uint8_t)(tmp % (seq_t)10);
                cnt0[d]++;
                tmp /= (seq_t)10;
            }
        }

        /* copy counts for canonical calculation */
        uint8_t cnt[10];
        for (int i = 0; i < 10; ++i) cnt[i] = cnt0[i];

        /* compute canonical minimal value of this digit multiset */
        int zeros = cnt[0];
        cnt[0] = 0;                    /* zeros handled separately */

        int dmin = 1;
        while (dmin <= 9 && cnt[dmin] == 0) ++dmin;

        seq_t canon;
        if (dmin <= 9) {
            canon = dmin;
            cnt[dmin]--;
            for (int z = 0; z < zeros; ++z) canon = canon * (seq_t)10;
            for (int d = 0; d <= 9; ++d) {
                while (cnt[d]--) {
                    canon = (canon * (seq_t)10) + d;
                }
            }
        } else {  /* only zeros, i.e., the number 0 */
            canon = (seq_t)0;
        }

        if (canon == current) {
            /* generate all permutations of the digits, in lexicographic order */
            uint8_t perm[32];
            int plen = 0;
            for (int d = 0; d <= 9; ++d) {
                for (int k = 0; k < (int)cnt0[d]; ++k) {
                    perm[plen++] = d;
                }
            }
            do {
                if (!(plen > 1 && perm[0] == 0)) {
                    seq_t val = digits_to_value(perm, plen);
                    if (produced == n) {
                        return val;
                    }
                    ++produced;
                }
            } while (next_permutation_arr(perm, plen));
        }
        ++current;
    }
}
