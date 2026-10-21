/*
 * A249606  Primes of the form 2k^2 + k + 2.
 * https://oeis.org/A249606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 2k^2 + k + 2, n >= 1
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 27, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides: bool is_prime_big(seq_t n); */

static seq_t *primes_of_form = NULL;   /* cache of found primes */
static size_t primes_cap     = 0;      /* allocated capacity */
static size_t primes_len     = 0;      /* number of primes stored */
static idx_t   next_k        = 0;      /* next k to test */

static void ensure_capacity(void)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(1); /* allocation failure */
        if (primes_of_form) {
            memcpy(new_arr, primes_of_form, primes_len * sizeof(seq_t));
            free(primes_of_form);
        }
        primes_of_form = new_arr;
        primes_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0, but return 0 */

    /* compute primes until we have at least n primes */
    while (primes_len < n) {
        idx_t       k       = next_k++;
        seq_t       k_seq   = (seq_t)k;
        seq_t       p       = 2 * k_seq * k_seq + k_seq + 2; /* 2k^2 + k + 2 */

        if (is_prime_big(p)) {
            ensure_capacity();
            primes_of_form[primes_len++] = p;
        }
    }

    return primes_of_form[n - 1];
}
