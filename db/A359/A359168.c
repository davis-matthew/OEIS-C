/*
 * A359168  Numbers k for which there is an odd number of prime factors (when
 *          counted with multiplicity) in k*sigma(k), where sigma is the sum
 *          of divisors function.
 * https://oeis.org/A359168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by testing parity of Ω(k) and Ω(σ(k)) for each integer k
   and storing the values of k when the parities differ (i.e. when
   k·σ(k) has an odd number of prime factors counted with multiplicity). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence values */
    static seq_t *terms      = NULL;
    static size_t  capacity  = 0;
    static size_t  count     = 0;
    static idx_t   current_k = 1;   /* last k checked */

    while (count < n) {
        /* next candidate */
        current_k++;

        /* σ(k) */
        seq_t sigma = sigma_of(current_k);

        /* Ω(k) and Ω(σ(k)) */
        seq_t omega_k   = bigomega_seq((seq_t)current_k);
        seq_t omega_sig = bigomega_seq(sigma);

        /* Store the k if parities differ */
        if ( ((omega_k & 1u) ^ (omega_sig & 1u)) == 1u ) {
            if (count == capacity) {
                size_t new_cap = (capacity == 0) ? 64 : capacity * 2;
                terms = seq_realloc(terms, capacity, new_cap);
                capacity = new_cap;
            }
            terms[count++] = (seq_t)current_k;
        }
    }

    return terms[n - 1];
}
