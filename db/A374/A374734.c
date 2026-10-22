/*
 * A374734  a(n) = a(n-1) + rotate(a(n-1), n-1 digits left) with a(1) = 1.
 * https://oeis.org/A374734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 49, u128 n <= 90,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1
 *          a(n) = a(n-1) + rotate(a(n-1), -(n-1))
 *          where rotate(x, -k) rotates the decimal digits of x left by k places
 *          (k is reduced modulo the number of digits of x).
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* current capacity of cache (index 0 unused) */
    static idx_t max_n = 0;      /* largest n whose value has been computed */

    if (n < 1) {
        return 0;   /* undefined for n < 1, but never called with such n */
    }

    /*--- initialise cache if first call ---*/
    if (cap == 0) {
        /* ensure capacity for at least indices 0..n */
        cap = 4;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return 0;
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        /* base value a(1) = 1 */
        cache[1] = 1;
        max_n = 1;
    }

    /*--- allocate more space if necessary ---*/
    if (n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;
        /* zero the newly allocated part */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = 0;
        cache = tmp;
        cap = newcap;
    }

    /*--- if already computed, return it ---*/
    if (n <= max_n) {
        return cache[n];
    }

    /*--- compute terms from max_n+1 up to n ---*/
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        /* number of decimal digits of prev */
        int digits = digit_len(prev);
        if (digits <= 0) digits = 1;   /* safety, should not happen */

        /* rotation amount */
        idx_t k = (i - 1) % (idx_t)digits;   /* k in [0, digits-1] */

        seq_t rotated;
        if (k == 0) {
            rotated = prev;
        } else {
            /* low part = last (digits - k) digits of prev */
            seq_t pow_low = pow10((idx_t)(digits - k));
            seq_t low = prev % pow_low;
            seq_t high = prev / pow_low;
            rotated = low * pow10(k) + high;
        }

        cache[i] = prev + rotated;
    }

    max_n = n;
    return cache[n];
}
