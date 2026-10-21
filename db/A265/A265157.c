/*
 * A265157  Number of 2's in the base-3 representation of 2^n - 1.
 * https://oeis.org/A265157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit 2's in the base‑3 representation of 2ⁿ −
 *            1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for incremental computation */
    static seq_t *cnt = NULL;        /* counts of 2's for each n */
    static size_t  cnt_cap = 0;      /* current capacity of cnt */
    static idx_t   max_n   = 0;      /* highest n computed so far */

    static uint8_t *digits   = NULL; /* base‑3 digits of 2^max_n − 1 */
    static size_t   digits_len = 0;   /* number of used digits */
    static size_t   digits_cap = 0;   /* capacity of digits array */

    /* Initialization on first call */
    if (cnt == NULL) {
        cnt_cap   = 1;
        cnt       = (seq_t*)malloc(cnt_cap * sizeof(seq_t));
        cnt[0]    = 0;                 /* a(0) = 0 */
        max_n     = 0;

        digits_cap = 1;
        digits_len = 1;
        digits     = (uint8_t*)malloc(digits_cap * sizeof(uint8_t));
        digits[0]  = 0;                 /* 2^0 − 1 = 0 */
    }

    /* Return cached value if already computed */
    if (n <= max_n)
        return cnt[n];

    /* Extend count array if necessary */
    if ((size_t)(n + 1) > cnt_cap) {
        size_t new_cap = cnt_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        cnt = (seq_t*)realloc(cnt, new_cap * sizeof(seq_t));
        cnt_cap = new_cap;
    }

    /* Compute successive terms from max_n up to n */
    for (idx_t k = max_n; k < n; ++k) {
        /* x_{k+1} = 2 * x_k + 1   (x_k is stored in base‑3 in digits[]) */

        /* Step 1: multiply by 2 */
        uint8_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint8_t tmp = digits[i] * 2 + carry;
            digits[i] = tmp % 3;
            carry     = tmp / 3;
        }
        if (carry) {
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                digits = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
            }
            digits[digits_len++] = carry;
        }

        /* Step 2: add 1 */
        carry = 1;
        size_t i = 0;
        while (carry && i < digits_len) {
            uint8_t tmp = digits[i] + carry;
            digits[i] = tmp % 3;
            carry     = tmp / 3;
            ++i;
        }
        if (carry) {
            if (digits_len == digits_cap) {
                digits_cap *= 2;
                digits = (uint8_t*)realloc(digits, digits_cap * sizeof(uint8_t));
            }
            digits[digits_len++] = carry;
        }

        /* Step 3: count digit 2's in current representation */
        seq_t c = 0;
        for (size_t j = 0; j < digits_len; ++j)
            if (digits[j] == 2)
                ++c;
        cnt[k + 1] = c;
    }

    max_n = n;
    return cnt[n];
}
