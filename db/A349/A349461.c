/*
 * A349461  Primes of the form m^2 + 9*m + 81.
 * https://oeis.org/A349461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349461 : primes of the form m^2 + 9*m + 81, in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence offset is 1; we can safely return 0 for n==0. */
        return (seq_t)0;
    }

    /* Static state: prime table, its size, capacity and last processed k. */
    static seq_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static idx_t last_k = 0;

    /* Ensure there is enough space to hold (primes_len+1) elements. */
    if (last_k == 0 && primes_cap == 0) {
        primes_cap = 64;
        primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
    }

    while (primes_len < n) {
        /* For a given k we have two m values:
           m_left  = -4 - k   (negative side)
           m_right = -5 + k   (mirror side, may be negative or positive)  */
        idx_t k = last_k;
        int64_t m_left  = -4 - (int64_t)k;
        int64_t m_right = -5 + (int64_t)k;

        /* Compute p = m^2 + 9*m + 81 using 64‑bit arithmetic.  */
        int64_t v_left  = m_left  * m_left  + 9 * m_left  + 81;
        int64_t v_right = m_right * m_right + 9 * m_right + 81;

        /* Test left value */
        seq_t p_left = (seq_t)v_left;
        if (is_prime_big(p_left)) {
            if (primes_len == 0 || p_left != primes[primes_len - 1]) {
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                }
                primes[primes_len++] = p_left;
                if (primes_len >= n) break;
            }
        }

        /* Test right value (may be equal to left). */
        seq_t p_right = (seq_t)v_right;
        if (is_prime_big(p_right)) {
            if (primes_len == 0 || p_right != primes[primes_len - 1]) {
                if (primes_len == primes_cap) {
                    primes_cap <<= 1;
                    primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
                }
                primes[primes_len++] = p_right;
                if (primes_len >= n) break;
            }
        }

        ++last_k;
    }

    return primes[n - 1];
}
