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
 * Helpers:   count_prime_factors, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by testing parity of Ω(k) and Ω(σ(k)) for each integer k
   and storing the values of k when the parities differ (i.e. when
   k·σ(k) has an odd number of prime factors counted with multiplicity). */

#include "../utils/sigma_of.h"
#include "../utils/count_prime_factors.h"
#include "../utils/seq_realloc.h"

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
        seq_t omega_k   = count_prime_factors((seq_t)current_k);
        seq_t omega_sig = count_prime_factors(sigma);

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
