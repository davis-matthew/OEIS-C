/*
 * A290008  Prime numbers of the form 6^k - 5.
 * https://oeis.org/A290008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 6^k - 5 (k ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* for size_t */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/seq_realloc.h"    /* seq_t* seq_realloc(seq_t*,size_t,size_t) */

seq_t a(idx_t n)
{
    /* Static state to allow non‑sequential calls. */
    static seq_t *primes      = NULL;
    static size_t primes_len  = 0;   /* number of primes stored */
    static size_t primes_cap  = 0;   /* allocated capacity */
    static idx_t  current_k   = 0;   /* current value of k (starts at 0) */
    static seq_t pow6         = 1;   /* 6^current_k, starts at 6^0 = 1 */

    while (primes_len < (size_t)n) {
        /* Advance to the next k and update 6^k. */
        current_k++;
        pow6 = pow6 * (seq_t)6;          /* compute 6^k */

        seq_t candidate = pow6 - (seq_t)5;   /* 6^k - 5 */

        if (is_prime_big(candidate)) {       /* test primality */

            /* Grow the array if needed. */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
                primes = tmp;
                primes_cap = new_cap;
            }

            primes[primes_len++] = candidate;
        }
    }

    return primes[n - 1];
}
