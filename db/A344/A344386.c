/*
 * A344386  Decimal expansion of sqrt(53)/7.
 * https://oeis.org/A344386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   big_compare, big_copy, big_ensure, big_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS Sequence A344386: Decimal expansion of sqrt(53)/7.
 * The function returns the nth decimal digit (base 10) of sqrt(53)/7
 * with indexing starting at 1 (a(1) = 1, the integer part).
 * The algorithm performs a digit–by–digit square–root extraction for
 * sqrt(53) and simultaneously divides that number by 7 using ordinary
 * decimal long division.  Big integers are stored as arrays of uint32_t
 * limbs in base 1 000 000 000 to avoid overflow on 64‑bit builds.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_compare.h"
#include "../utils/big_copy.h"
#include "../utils/big_ensure.h"
#include "../utils/big_trim.h"

#define WORD_BASE 1000000000U   /* 1e9 */
#define WORD_CAP  1200          /* enough for >10000 decimal digits */

static inline void big_mul_small(const uint32_t *a, size_t a_len,
                                 uint32_t m, uint32_t *res, size_t *res_len) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a_len; i++) {
        uint64_t prod = (uint64_t)a[i] * m + carry;
        res[i] = (uint32_t)(prod % WORD_BASE);
        carry = prod / WORD_BASE;
    }
    if (carry) {
        res[i++] = (uint32_t)carry;
    }
    *res_len = big_trim(i, res);
}

static inline void big_add_small(uint32_t *a, size_t *len,
                                 uint32_t add) {
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < *len) {
        uint64_t sum = (uint64_t)a[i] + carry;
        a[i] = (uint32_t)(sum % WORD_BASE);
        carry = sum / WORD_BASE;
        i++;
    }
    if (carry) {
        a[i++] = (uint32_t)carry;
        *len = big_trim(i, a);
    }
}

static inline void big_subtract(const uint32_t *a, size_t len_a,
                                const uint32_t *b, size_t len_b,
                                uint32_t *res, size_t *res_len) {
    /* assume a >= b */
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < len_a; i++) {
        int64_t diff = (int64_t)a[i] - borrow - (i < len_b ? b[i] : 0);
        if (diff < 0) {
            diff += WORD_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[i] = (uint32_t)diff;
    }
    *res_len = big_trim(i, res);
}

static inline void big_inc(uint32_t *a, size_t *len, uint32_t inc) {
    /* increment by small integer inc (0-9) */
    big_add_small(a, len, inc);
}

static inline void big_mul_small_inc(uint32_t *a, size_t *len,
                                     uint32_t m, uint32_t inc) {
    /* a = a * m + inc */
    big_mul_small(a, *len, m, a, len);
    big_add_small(a, len, inc);
}

static inline void big_mul_small_inplace(uint32_t *a, size_t *len,
                                         uint32_t m) {
    /* a = a * m */
    big_mul_small(a, *len, m, a, len);
}

static inline void big_mul_small_copy(uint32_t *src, size_t src_len,
                                      uint32_t m,
                                      uint32_t *dst, size_t *dst_len) {
    big_mul_small(src, src_len, m, dst, dst_len);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* off‑set is 1 */

    /* static cache of already computed digits (a(1)=1 stored at index 0) */
    static uint8_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* big‑integer buffers for the square‑root extraction of sqrt(53) */
    static uint32_t root_words[WORD_CAP];
    static size_t root_len = 0;
    static uint32_t rem_words[WORD_CAP];
    static size_t rem_len = 0;

    /* digit of the division by 7: current remainder modulo 7 */
    static uint8_t rem_div = 0;

    /* temporary buffers used inside the loop */
    static uint32_t t_words[WORD_CAP];
    static size_t t_len = 0;
    static uint32_t prod_words[WORD_CAP];
    static size_t prod_len = 0;

    /* one‑time initialization */
    if (cache_len == 0) {
        /* sqrt(53) integer part */
        root_len = 1;
        root_words[0] = 7;

        /* remainder of sqrt extraction: 53 - 7^2 = 4 */
        rem_len = 1;
        rem_words[0] = 4;

        rem_div = 0;

        cache_cap = 32;
        cache = (uint8_t *)malloc(cache_cap * sizeof(uint8_t));
        if (!cache) return 0;                /* allocation failure */
        cache[0] = 1;                        /* a(1) = 1 */
        cache_len = 1;
    }

    /* generate digits until we reach the requested n */
    while (cache_len < (size_t)n) {
        /* step 1: bring down two zeros for the next decimal digit of sqrt(53) */
        big_mul_small(rem_words, rem_len, 100, rem_words, &rem_len);

        /* step 2: compute t = 20 * root */
        big_mul_small(root_words, root_len, 20, t_words, &t_len);

        /* step 3: find the largest d (0..9) s.t. (t + d)*d <= rem */
        int d = 0;
        for (int cand = 9; cand >= 0; cand--) {
            /* temp = t + cand */
            big_copy(t_words, t_len, prod_words, &prod_len);
            big_add_small(prod_words, &prod_len, (uint32_t)cand);

            /* product = (t + cand) * cand */
            big_mul_small(prod_words, prod_len, (uint32_t)cand,
                          prod_words, &prod_len);

            /* compare with rem */
            if (big_compare(prod_words, prod_len, rem_words, rem_len) <= 0) {
                d = cand;
                /* rem = rem - product */
                big_subtract(rem_words, rem_len,
                             prod_words, prod_len,
                             rem_words, &rem_len);
                break;
            }
        }

        /* step 4: update root = root * 10 + d */
        big_mul_small_inc(root_words, &root_len, 10, (uint32_t)d);

        /* step 5: perform long division by 7 on the decimal digits */
        unsigned int new_remainder = rem_div * 10u + (unsigned int)d;
        uint8_t q = (uint8_t)(new_remainder / 7u);
        rem_div = (uint8_t)(new_remainder % 7u);

        /* store the quotient digit */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(cache, cache_cap * sizeof(uint8_t));
            if (!tmp) { free(cache); cache = NULL; cache_len = 0; return (seq_t)q; }
            cache = tmp;
        }
        cache[cache_len++] = q;
    }

    return (seq_t)cache[n - 1];
}
