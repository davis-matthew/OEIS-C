/*
 * A336373  Primes prime(k) such that gcd(k, prime(k)+prime(k-1)) > 1.
 * https://oeis.org/A336373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd, get_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336373
   a(n) = prime(n) such that gcd(n, prime(n)+prime(n-1)) > 1
   Implementation uses incremental generation of primes via get_prime().
   The sequence values are stored in a static dynamic array so that
   subsequent calls to a() can resume efficiently, independent of call
   order.
*/
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static storage for the sequence values and state */
    static seq_t *seq      = NULL;   /* array of qualifying primes */
    static size_t seq_cap  = 0;      /* allocated capacity */
    static size_t seq_len  = 0;      /* number of values stored */
    static idx_t  last_k   = 1;      /* last examined prime index */

    /* Return cached value if we already have it */
    if (n <= seq_len)
        return seq[n - 1];

    /* Grow the array when necessary */
    if (n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
        seq   = tmp;
        seq_cap = new_cap;
    }

    /* Scan successive prime indices until we have n terms */
    while (seq_len < n) {
        ++last_k;                          /* next index k */

        /* Pull kth and (k‑1)th primes (use uint32_t from get_prime()) */
        seq_t pk     = (seq_t)get_prime(last_k);
        seq_t pk_prev = (seq_t)get_prime(last_k - 1);

        /* Compute gcd(k, pk + pk_prev) */
        seq_t sum = pk + pk_prev;
        seq_t g   = gcd((seq_t)last_k, sum);

        /* If gcd > 1, store the prime */
        if (g > (seq_t)1) {
            seq[seq_len++] = pk;
        }
    }

    return seq[n - 1];
}
