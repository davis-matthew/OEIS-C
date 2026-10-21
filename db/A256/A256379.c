/*
 * A256379  Cumulative sum for n in base 10 when alternately adding and
 *          subtracting each digit of a particular value.
 * https://oeis.org/A256379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} M(k) M(k) = Sum_{m=1..digits(k)}
 *            (-1)^(count(k,m)+1) * digit_m (count(k,m) is the running total
 *            of that digit up to number k)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache[i] stores a(i); index 0 is a(0)=0 (unused by OEIS but convenient) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;           /* largest index computed so far     */
    static uint32_t digit_counts[10] = {0}; /* occurrences of each decimal digit */

    if (n > cache_size) {
        size_t new_size = (size_t)n + 1;    /* include index 0            */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* unlikely failure           */
        cache = tmp;

        /* initialise base of the cache if it did not exist before */
        if (cache_size == 0 && n >= 0)
            cache[0] = (seq_t)0;

        /* compute terms that are still missing */
        for (size_t i = cache_size + 1; i <= n; ++i) {
            idx_t k = (idx_t)i;

            /* extract decimal digits of k (most‑significant first) */
            uint8_t digits[20];
            size_t len = 0;
            if (k == 0) {
                digits[len++] = 0;
            } else {
                while (k > 0) {
                    digits[len++] = (uint8_t)(k % 10);
                    k /= 10;
                }
                for (size_t l = 0; l < len / 2; ++l) {
                    uint8_t tmp = digits[l];
                    digits[l] = digits[len - 1 - l];
                    digits[len - 1 - l] = tmp;
                }
            }

            /* compute M(k) */
            seq_t mk = (seq_t)0;
            for (size_t pos = 0; pos < len; ++pos) {
                uint8_t d = digits[pos];
                digit_counts[d]++;                    /* new total count */

                seq_t digit = (seq_t)d;
                if (digit_counts[d] & 1U) {           /* odd count → add   */
                    mk += digit;
                } else {                              /* even count → sub  */
                    mk -= digit;
                }
            }

            cache[i] = cache[i - 1] + mk;
        }
        cache_size = n;
    }

    return cache[n];
}
