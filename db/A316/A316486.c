/*
 * A316486  Squares whose arithmetic mean of digits is 6 (i.e., the sum of
 *          digits is 6 times the number of digits).
 * https://oeis.org/A316486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 749, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A316486: squares whose arithmetic mean of digits is 6
 * i.e. digit_sum(square) == 6 * digit_count(square), with the
 * number of digits divisible by 3.
 * The function performs a brute‑force search over integers,
 * caching found squares so that repeated calls are fast.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found squares   */
    static size_t sz   = 0;       /* number of terms cached  */
    static size_t cap  = 0;       /* allocated capacity      */
    static idx_t k     = 1;       /* next integer to test    */

    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    while (sz < n) {
        seq_t val = (seq_t)k * k;      /* k^2 */
        seq_t temp = val;
        int len = 0;
        seq_t sum = 0;

        /* compute digit sum and length */
        while (temp > 0) {
            sum += temp % 10;
            temp /= 10;
            ++len;
        }

        /* check the condition */
        if (len % 3 == 0 && sum == ((seq_t)len) * 6) {
            if (sz == cap) {            /* grow cache if needed */
                size_t new_cap = cap ? cap * 2 : 4;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cap = new_cap;
            }
            cache[sz++] = val;
        }
        ++k;
    }
    return cache[n - 1];
}
