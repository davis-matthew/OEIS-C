/*
 * A358509  Sum of decimal digits of (3^n - 1)/2 (A003462).
 * https://oeis.org/A358509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit_sum((3^n - 1)/2)
 * Range:     u8 n <= 105, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Big‑digit representation (little‑endian, base 10) of 3^k. */
static uint8_t *pow_digits = NULL;   /* digits of 3^k */
static size_t   pow_len    = 0;       /* number of used digits */
static size_t   pow_cap    = 0;       /* allocated capacity */

/* Temporary buffer for intermediate calculations. */
static uint8_t *temp_digits = NULL;
static size_t   temp_cap    = 0;

/* Cache of a(k) values. */
static seq_t *sum_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* What power of 3 we have currently stored. */
static idx_t current_k = 0;

/* Ensure pow_digits has capacity at least 'need'. */
static void need_pow_cap(size_t need)
{
    if (pow_cap < need) {
        size_t newcap = (pow_cap > 0) ? pow_cap * 2 : 16;
        if (newcap < need) newcap = need;
        uint8_t *tmp = (uint8_t *)realloc(pow_digits, newcap * sizeof(uint8_t));
        pow_digits = tmp;
        pow_cap = newcap;
    }
}

/* Ensure temp_digits has at least 'need' capacity. */
static void need_temp_cap(size_t need)
{
    if (temp_cap < need) {
        size_t newcap = (temp_cap > 0) ? temp_cap * 2 : 16;
        if (newcap < need) newcap = need;
        uint8_t *tmp = (uint8_t *)realloc(temp_digits, newcap * sizeof(uint8_t));
        temp_digits = tmp;
        temp_cap = newcap;
    }
}

/* Ensure sum_cache has at least 'need' capacity. */
static void need_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t newcap = (cache_cap > 0) ? cache_cap * 2 : 16;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(sum_cache, newcap * sizeof(seq_t));
        sum_cache = tmp;
        cache_cap = newcap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* ---------- Initialisation ---------- */
    if (pow_len == 0) {
        /* 3^0 = 1 */
        need_pow_cap(16);
        pow_digits[0] = 1;
        pow_len = 1;

        /* Cache a(0) = 0 */
        need_cache_cap(16);
        sum_cache[0] = (seq_t)0;
        cache_len = 1;

        /* Temporary buffer */
        need_temp_cap(pow_cap);

        current_k = 0;
    }

    /* ---------- Return cached value if available ---------- */
    if ((size_t)n < cache_len)
        return sum_cache[(size_t)n];

    /* ---------- Compute values up to n ---------- */
    for (idx_t k = current_k; k < n; ++k) {
        /* 1. Multiply current 3^k by 3 to get 3^(k+1). */
        uint8_t carry = 0;
        for (size_t i = 0; i < pow_len; ++i) {
            uint16_t prod = (uint16_t)pow_digits[i] * 3 + carry;
            pow_digits[i] = prod % 10;
            carry = prod / 10;
        }
        while (carry) {
            need_pow_cap(pow_len + 1);
            pow_digits[pow_len++] = carry % 10;
            carry /= 10;
        }

        /* 2. Copy into temp buffer for manipulation. */
        need_temp_cap(pow_len);
        for (size_t i = 0; i < pow_len; ++i)
            temp_digits[i] = pow_digits[i];

        /* 3. Subtract 1. */
        size_t i = 0;
        while (i < pow_len) {
            if (temp_digits[i] > 0) {
                temp_digits[i]--;
                break;
            }
            temp_digits[i] = 9;
            ++i;
        }

        /* 4. Divide by 2 (decimal division). */
        uint8_t carryd = 0;
        size_t len2 = pow_len;
        for (size_t j = len2; j > 0; --j) {
            size_t idx = j - 1;
            uint16_t val = carryd * 10 + temp_digits[idx];
            temp_digits[idx] = val / 2;
            carryd = val % 2;
        }

        /* 5. Trim leading zeros (most significant side). */
        while (len2 > 1 && temp_digits[len2 - 1] == 0)
            --len2;

        /* 6. Sum the decimal digits. */
        unsigned sum = 0;
        for (size_t j = 0; j < len2; ++j)
            sum += temp_digits[j];

        /* 7. Cache the result. */
        if (cache_len <= (size_t)(k + 1)) {
            need_cache_cap((size_t)(k + 2));
            sum_cache[cache_len++] = (seq_t)sum;
        } else {
            sum_cache[(size_t)(k + 1)] = (seq_t)sum;
        }
    }

    current_k = n;
    return sum_cache[(size_t)n];
}
