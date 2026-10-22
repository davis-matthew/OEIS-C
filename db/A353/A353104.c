/*
 * A353104  Base-4 representation of A007908(n).
 * https://oeis.org/A353104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Base-4 representation of the concatenation of 1..n,
 *            interpreted as a decimal number (A007090(A007908(n)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* -------- 1. build the decimal concatenation string  -------- */
    size_t cap = n * 6 + 10;                     /* safe upper bound */
    char *buf = (char *)malloc(cap);
    if (!buf) return 0;
    char *p = buf;
    for (idx_t i = 1; i <= n; ++i) {
        int len = sprintf(p, "%llu", (unsigned long long)i);
        p += (size_t)len;
    }
    *p = '\0';
    size_t str_len = (size_t)(p - buf);

    /* -------- 2. convert string to limbs in base 1e9  -------- */
    size_t limbs_est = (str_len + 8) / 9;
    uint32_t *limbs = (uint32_t *)malloc(limbs_est * sizeof(uint32_t));
    size_t limbs_len = 0;

    for (size_t pos = str_len; pos > 0; ) {
        size_t start   = (pos > 9 ? pos - 9 : 0);
        size_t chunk_len = pos - start;
        uint32_t limb = 0;
        for (size_t k = 0; k < chunk_len; ++k)
            limb = limb * 10u + (uint32_t)(buf[start + k] - '0');
        limbs[limbs_len++] = limb;
        pos = start;
    }

    /* -------- 3. divide by 4 repeatedly, collecting base‑4 digits -------- */
    size_t max_base4_digits = str_len * 2 + 10;   /* safe estimate */
    uint8_t *base4digits = (uint8_t *)malloc(max_base4_digits);
    size_t digit_idx = 0;

    while (limbs_len > 0) {
        uint32_t rem = 0;
        for (size_t i = limbs_len; i > 0; --i) {
            uint64_t cur = ((uint64_t)rem) * 1000000000ULL + (uint64_t)limbs[i - 1];
            limbs[i - 1] = (uint32_t)(cur / 4);
            rem = (uint32_t)(cur % 4);
        }
        base4digits[digit_idx++] = (uint8_t)rem;
        while (limbs_len > 0 && limbs[limbs_len - 1] == 0)
            --limbs_len;
    }

    /* -------- 4. assemble the result from most significant digit -------- */
    seq_t result = 0;
    for (size_t i = digit_idx; i > 0; --i)
        result = result * (seq_t)10 + (seq_t)base4digits[i - 1];

    /* -------- 5. cleanup -------- */
    free(buf);
    free(limbs);
    free(base4digits);

    return result;
}
