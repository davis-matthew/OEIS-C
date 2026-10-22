/*
 * A352462  Numbers k with the property that k ends in the sum of the digits
 *          of k.
 * https://oeis.org/A352462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Numbers k such that k “ends in” the sum of its digits. That is,
 *            k ≡ sum_digits(k) (mod 10^d), where d = number of decimal digits
 *            of sum_digits(k)
 * Range:     u8 n <= 9, u16 n <= 729, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t pow10_arr[20];

static void init_pow10(void) {
    pow10_arr[0] = (seq_t)1;
    for (int i = 1; i < 20; ++i)
        pow10_arr[i] = pow10_arr[i - 1] * (seq_t)10;
}

static seq_t digit_sum(seq_t x) {
    seq_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

static int digit_len_seq_t(seq_t x) {
    int len = 0;
    while (x > 0) {
        ++len;
        x /= 10;
    }
    return len;
}

static void ensure_cache_capacity(idx_t needed, seq_t **cache, size_t *cap) {
    if (needed <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : *cap;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static bool pow10_ready = false;
    static idx_t current_k = 1;      /* next integer to test */

    if (!pow10_ready) {
        init_pow10();
        pow10_ready = true;
    }
    if (n == 0)
        return (seq_t)0;             /* undefined for offset 1, but harmless */

    while (cache_len < n) {
        seq_t k = (seq_t)current_k;
        seq_t sum = digit_sum(k);
        int d = digit_len_seq_t(sum);
        /* d >= 1 because k ≥ 1 ⇒ sum ≥ 1 */
        seq_t mod = k % pow10_arr[d];
        if (mod == sum) {
            ensure_cache_capacity((idx_t)(cache_len + 1), &cache, &cache_cap);
            cache[cache_len++] = k;
        }
        ++current_k;
    }
    return cache[n - 1];
}
