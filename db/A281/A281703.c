/*
 * A281703  Numbers k such that A000005(k) = A000005(A000326(k)).
 * https://oeis.org/A281703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that divisor_count(k) =
 *            divisor_count(k*(3*k-1)/2)
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static caching of computed terms */
    static seq_t *cache = NULL;    /* array of terms */
    static size_t used  = 0;       /* number of terms stored */
    static size_t cap   = 0;       /* capacity of cache */
    static idx_t next_k = 1;       /* next k to test */

    while (used < n) {
        seq_t k   = (seq_t)next_k;
        seq_t pent = k * (((seq_t)next_k * 3) - 1) / 2;

        /* Count divisors of k and its pentagonal number */
        seq_t tau_k    = (seq_t)divisor_count_u64((uint64_t)next_k);
        seq_t tau_pent = (seq_t)divisor_count_u64((uint64_t)pent);

        if (tau_k == tau_pent) {
            if (used == cap) {
                size_t new_cap = (cap == 0) ? 128 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation error */
                cache = tmp;
                cap = new_cap;
            }
            cache[used++] = next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
