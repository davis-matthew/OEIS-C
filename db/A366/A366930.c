/*
 * A366930  a(n) is the smallest odd composite k such that n^((k+1)/2) == n
 *          (mod k).
 * https://oeis.org/A366930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest odd composite k such that n^((k+1)/2) ≡ n (mod
 *            k)
 * Range:     u8 n <= 1, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Pre‑computed odd composite list up to 1729 (largest needed k) */
    static seq_t *odd_comps = NULL;
    static size_t odd_comp_cnt = 0;

    if (!odd_comps) {
        const idx_t NMAX = 1729;
        const size_t ARRAY_LEN = (NMAX + 1) / 2;   /* enough space for all odd numbers */

        odd_comps = (seq_t *)calloc(ARRAY_LEN, sizeof(seq_t));
        if (!odd_comps)
            return 0;  /* allocation failed – should not happen in tests */

        for (idx_t k = 9; k <= NMAX; k += 2) {
            if (!is_prime_big(k)) {          /* odd composite */
                odd_comps[odd_comp_cnt++] = (seq_t)k;
            }
        }
    }

    seq_t val = (seq_t)n;

    for (size_t i = 0; i < odd_comp_cnt; ++i) {
        seq_t k = odd_comps[i];
        uint64_t exp = (uint64_t)((k + 1) / 2);
        seq_t r = mod_pow(val, exp, k);
        if (r == (val % k))
            return k;
    }

    /* Theoretical guarantee: a(n) ≤ 1729, so we should never reach here */
    return 0;
}
