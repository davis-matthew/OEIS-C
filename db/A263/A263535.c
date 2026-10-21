/*
 * A263535  a(1) = 1; thereafter a(n) = a(n-1) + d_1^1 + d_2^2 + d_3^3 + ...,
 *          where d_1 d_2 d_3 ... is the decimal expansion of a(n-1).
 * https://oeis.org/A263535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + d_1^1 + d_2^2 + d_3^3 + …, a(1) = 1, where d_i
 *            are the decimal digits of a(n-1) from most significant up to
 *            least
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 83, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   digit_len, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* helper declarations – the build system will provide the headers */
#include "../utils/digit_len.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined; harmless */

    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cur_max = 0;            /* largest n for which cache[1..cur_max] is computed */

    /* ensure the cache is large enough */
    if (cache_cap <= n) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure – unlikely in tests */
        cache = tmp;
    }

    /* initialise a(1) once */
    if (cur_max == 0) {
        cache[1] = 1;
        cur_max = 1;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];

        /* get number of decimal digits of prev */
        int len = digit_len(prev);
        if (len <= 0) len = 1;           /* safeguard for 0, though it never occurs */

        /* extract digits into a temporary array (least‑significant first) */
        uint8_t *digits = (uint8_t *)malloc(len * sizeof(uint8_t));
        seq_t tmp = prev;
        int pos = 0;
        while (tmp > 0) {
            digits[pos++] = (uint8_t)(tmp % 10);  /* convert small seq_t to uint8_t */
            tmp /= 10;
        }
        /* pos == len for non‑zero numbers */

        /* compute the digit‑power sum (most significant digit has exponent 1) */
        seq_t add = 0;
        for (int d = 0; d < len; ++d) {
            uint8_t digit = digits[len - 1 - d];  /* most significant first */
            if (digit) {
                add += pow_uint(digit, d + 1);    /* exponent = position counting from 1 */
            }
        }

        free(digits);
        cache[i] = prev + add;
    }

    cur_max = n;
    return cache[n];
}
