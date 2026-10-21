/*
 * A292069  Primes of the form (24^k - 1) / 23.
 * https://oeis.org/A292069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (24^k - 1)/23 where k is the n‑th index for which that
 *            value is prime. The values are produced by iterating k and
 *            testing primality of the recurrence A_k = 24*A_(k-1) + 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   is_prime, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static state */
    static idx_t       current_k      = 1;           /* current exponent k   */
    static seq_t       current_value  = 1;            /* A_k = (24^k-1)/23     */
    static seq_t *     primes         = NULL;         /* array of found primes */
    static size_t      primes_cap     = 0;            /* capacity of array     */
    static size_t      prime_count    = 0;            /* number of primes     */

    /* grow primes array if necessary */
    if (prime_count >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
        if (!tmp) return 0;          /* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }

    /* generate primes until we have at least n */
    while (prime_count < (size_t)n) {
        ++current_k;
        /* next A_k = 24*A_(k-1) + 1 */
        current_value = current_value * 24 + 1;

        /* skip composite k (they cannot yield prime A_k) */
        if (!is_prime(current_k))
            continue;

        if (is_prime_big(current_value)) {
            primes[prime_count++] = current_value;
        }
    }

    return primes[n-1];
}
