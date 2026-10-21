/*
 * A288041  Numbers k such that prime(k) + prime(k+1) + ... + prime(k+4) is
 *          prime.
 * https://oeis.org/A288041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that
 *            prime(k)+prime(k+1)+prime(k+2)+prime(k+3)+prime(k+4) is prime
 * Range:     u8 n <= 8, u16 n <= 834, u32 n <= 140, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 4, a(3) = 5, a(4) = 6, ... (offset 1) */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state – cached values and next k to test */
    static seq_t *values = NULL;
    static size_t values_cap = 0;
    static size_t values_len = 0;
    static idx_t next_k = 1;   /* next k to examine */

    /* Grow the result array if needed */
    if (values_len < (size_t)n) {
        if (values_cap < (size_t)n) {
            size_t new_cap = values_cap ? values_cap * 2 : 16;
            if (new_cap < (size_t)n)
                new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed, abort */
                return 0;
            }
            values = tmp;
            values_cap = new_cap;
        }

        /* Continue testing primes until we have enough terms */
        while (values_len < (size_t)n) {
            seq_t sum = 0;
            sum += (seq_t) get_prime(next_k);
            sum += (seq_t) get_prime(next_k + 1);
            sum += (seq_t) get_prime(next_k + 2);
            sum += (seq_t) get_prime(next_k + 3);
            sum += (seq_t) get_prime(next_k + 4);

            if (is_prime_big(sum)) {
                values[values_len++] = next_k;
            }

            ++next_k;
        }
    }
    return values[n - 1];
}
