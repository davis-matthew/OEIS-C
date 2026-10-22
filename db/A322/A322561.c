/*
 * A322561  Digits of one of the two 17-adic integers sqrt(2) that is related
 *          to A322559.
 * https://oeis.org/A322561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322561: digits of the 17‑adic square root of 2 that ends with 6.
   Computed via iterative Hensel lifting in base 17.
   For each new digit a_k, let e be the coefficient of 17^k in the square of the
   current integer; then a_k = -(e * inv(12)) mod 17, where inv(12)=10 mod 17.
   The square digits are maintained in base 17 and carries are propagated only up
   to the required position.  This produces O(n²) time and O(n) memory,
   fast enough for n ≈ 10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for digits and square digits */
    static idx_t          max_n = 0;
    static uint8_t       *dig   = NULL;   /* digits a[i] (0 ≤ a < 17) */
    static int           *sq    = NULL;   /* digits of x² in base 17 */
    static size_t         sq_cap = 0;      /* allocated capacity for sq */
    static size_t         sq_len = 0;      /* current used length (highest index + 1) */

    /* Initialisation for n = 0 (first term) */
    if (max_n == 0) {
        if (n >= 0) {
            /* allocate digit array for a(0) */
            dig = (uint8_t*)malloc(sizeof(uint8_t));
            if (dig == NULL) return 0;
            dig[0] = 6;                     /* a(0) = 6 */

            /* allocate square array: 6² = 36 = 2 + 2·17 -> needs indices 0,1 */
            sq_cap = 4;
            sq = (int*)malloc(sq_cap * sizeof(int));
            if (sq == NULL) return 0;
            memset(sq, 0, sq_cap * sizeof(int));
            sq[0] = 2;         /* coefficient of 17⁰ in 36 */
            sq[1] = 2;         /* coefficient of 17¹ in 36 */
            sq_len = 2;         /* indices 0 and 1 used */

            max_n = 0;
        }
    }

    /* Expand cache to cover all terms up to n */
    if (n > max_n) {
        /* Resize digit array */
        size_t new_dig_cap = (size_t)n + 1;
        uint8_t *tmp_dig = (uint8_t*)realloc(dig, new_dig_cap * sizeof(uint8_t));
        if (tmp_dig == NULL) return 0;
        dig = tmp_dig;

        /* Resize square array */
        size_t needed_cap = 2 * ((size_t)n) + 10;
        if (sq_cap < needed_cap) {
            int *tmp_sq = (int*)realloc(sq, needed_cap * sizeof(int));
            if (tmp_sq == NULL) return 0;
            if (tmp_sq != sq) {
                for (size_t i = sq_cap; i < needed_cap; ++i) tmp_sq[i] = 0;
            } else {
                for (size_t i = sq_cap; i < needed_cap; ++i) tmp_sq[i] = 0;
            }
            sq = tmp_sq;
            sq_cap = needed_cap;
        }

        /* Compute digits from max_n+1 up to n */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* e = coefficient of 17^k in the current square (before update) */
            int e = sq[k] % 17;
            int a_k = (17 - (e * 10 % 17)) % 17;  /* a_k = -(e*10) mod 17 */

            dig[k] = (uint8_t)a_k;

            /* Ensure square array is large enough for indices up to 2k+1 */
            size_t required_len = 2 * (size_t)k + 2;
            if (sq_len < required_len) {
                for (size_t i = sq_len; i < required_len; ++i) sq[i] = 0;
                sq_len = required_len;
            }

            /* Add cross term 2*a_k*x */
            for (size_t i = 0; i < (size_t)k; ++i) {
                size_t idx = i + (size_t)k;
                sq[idx] += 2 * a_k * dig[i];
            }

            /* Add diagonal term a_k² at index 2k */
            sq[2 * (size_t)k] += a_k * a_k;

            /* Propagate carries from position k up to sq_len-1 */
            size_t idx = (size_t)k;
            while (idx < sq_len) {
                int carry = sq[idx] / 17;
                if (carry == 0) {
                    ++idx;
                    continue;
                }
                sq[idx] -= carry * 17;
                if (idx + 1 >= sq_len) {
                    /* Expand square array if necessary (should not happen) */
                    if (sq_len + 1 > sq_cap) {
                        size_t new_cap = sq_cap * 2;
                        if (new_cap < sq_len + 2) new_cap = sq_len + 2;
                        int *tmp = (int*)realloc(sq, new_cap * sizeof(int));
                        if (tmp == NULL) return 0;
                        for (size_t j = sq_cap; j < new_cap; ++j) tmp[j] = 0;
                        sq = tmp;
                        sq_cap = new_cap;
                    }
                    sq_len = idx + 2;
                }
                sq[idx + 1] += carry;
                ++idx;
            }
        }

        max_n = n;
    }

    return (seq_t)dig[n];
}
