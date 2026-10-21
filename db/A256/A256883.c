/*
 * A256883  Numbers divisible by prime(d+1) for each digit d of their base-3
 *          representation.
 * https://oeis.org/A256883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth non‑negative integer such that for each digit d
 *            in its base‑3 representation, the number is divisible by
 *            prime(d+1) (prime(1)=2, prime(2)=3, prime(3)=5). The
 *            implementation uses a brute‑force search with memoisation
 * Range:     u8 n <= 18, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of already computed sequence values */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;  /* allocated capacity */
    static size_t cache_len = 0;  /* number of stored terms   */
    static seq_t next_val   = 0;  /* next integer to test     */

    while (cache_len < n) {
        seq_t cur = next_val;

        /* Determine which base‑3 digits appear in cur */
        uint8_t mask = 0;          /* bit0: digit 0, bit1: digit 1, bit2: digit 2 */
        if (cur == 0) {
            mask = 1;              /* representation "0" contains digit 0 */
        } else {
            seq_t tmp = cur;
            while (tmp > 0) {
                int d = (int)(tmp % 3);
                mask |= 1U << d;
                tmp /= 3;
            }
        }

        /* Build the divisor: product of primes corresponding to present digits */
        int prod = 1;
        if (mask & 1U) prod *= 2;     /* prime(1) = 2 */
        if (mask & 2U) prod *= 3;     /* prime(2) = 3 */
        if (mask & 4U) prod *= 5;     /* prime(3) = 5 */

        /* Accept if cur is divisible by the product (always true for cur==0) */
        if (cur % (seq_t)prod == 0) {
            /* Ensure cache has space */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                     /* allocation failure */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }

        /* Advance to next integer */
        next_val = cur + 1;
    }

    return cache[n - 1];
}
