/*
 * A257162  Number of ordered ways to write n as floor(x/5) + floor(y/6) +
 *          floor(z/7), where x is a pentagonal number, y is a hexagonal
 *          number and z is a heptagonal number.
 * https://oeis.org/A257162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triplets (x y z) with * x = pentagonal
 *            number, y = hexagonal number, z = heptagonal number * such that
 *            floor(x/5)+floor(y/6)+floor(z/7) = n * The function computes
 *            this by brute‑force enumeration of all possible * floors up to
 *            n. The algorithm runs in O(K³) where K ≈ √n a...
 * Range:     u8 n <= 32, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   push_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/push_val.h"

seq_t a(idx_t n)
{
    idx_t *pent = NULL, *hex = NULL, *hept = NULL;
    size_t pent_len = 0, hex_len = 0, hept_len = 0;
    size_t pent_cap = 0, hex_cap = 0, hept_cap = 0;

    /* generate floor(P_k/5) for pentagonal numbers */
    for (idx_t k = 0; ; ++k) {
        seq_t pk = ((seq_t)k * ((seq_t)3 * k - 1)) / 2;
        seq_t val = pk / 5;
        if (val > n) break;
        push_val(&pent, &pent_len, &pent_cap, (idx_t)val);
    }

    /* generate floor(H_k/6) for hexagonal numbers */
    for (idx_t k = 0; ; ++k) {
        seq_t hk = ((seq_t)k * (2 * k - 1));   /* H_k = k(2k-1) */
        seq_t val = hk / 6;
        if (val > n) break;
        push_val(&hex, &hex_len, &hex_cap, (idx_t)val);
    }

    /* generate floor(P_{7,k}/7) for heptagonal numbers */
    for (idx_t k = 0; ; ++k) {
        seq_t hk = ((seq_t)k * ((seq_t)5 * k - 3)) / 2;  /* 5k^2-3k / 2 */
        seq_t val = hk / 7;
        if (val > n) break;
        push_val(&hept, &hept_len, &hept_cap, (idx_t)val);
    }

    seq_t count = 0;
    for (size_t i = 0; i < pent_len; ++i) {
        idx_t a_val = pent[i];
        for (size_t j = 0; j < hex_len; ++j) {
            idx_t b_val = hex[j];
            idx_t sum_ab = a_val + b_val;
            if (sum_ab > n) continue;
            idx_t target = n - sum_ab;
            for (size_t k = 0; k < hept_len; ++k) {
                if (hept[k] == target) {
                    ++count;
                }
            }
        }
    }

    free(pent);
    free(hex);
    free(hept);
    return count;
}
