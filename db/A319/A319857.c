/*
 * A319857  Difference between 4^n and the product of primes less than or
 *          equal to n.
 * https://oeis.org/A319857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 4^n – n# (n# = product of primes ≤ n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow, is_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp) */
#include "../utils/is_prime.h"   /* is_prime(idx_t n) */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return 0;

    /* Cached primorial values 0# … max_n# */
    static seq_t *prim = NULL;   /* array of primorials */
    static size_t max_n = 0;     /* largest n for which prim array is valid */

    if (n > max_n) {
        /* Allocate a new array that fits up to n */
        size_t new_cap = n + 1;
        seq_t *tmp = seq_alloc_new_array(new_cap);
        if (tmp == NULL) abort();        /* allocation failure */

        /* Copy old values */
        if (prim != NULL) {
            for (size_t i = 0; i <= max_n; ++i) tmp[i] = prim[i];
            seq_free_array(prim);   /* free old memory */
        }
        prim   = tmp;
        max_n  = n;
    }

    /* Compute primorials up to n if needed */
    for (size_t i = 2; i <= n; ++i) {
        if (prim[i] == 0) {            /* uninitialized entry */
            seq_t prev = prim[i - 1];
            if (is_prime((idx_t)i))
                prim[i] = prev * (seq_t)i;
            else
                prim[i] = prev;
        }
    }
    /* Ensure entries 0# and 1# are initialized */
    if (prim[0] == 0) prim[0] = 1;
    if (prim[1] == 0) prim[1] = 1;

    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    return pow4 - prim[n];
}
