/*
 * A292763  Numbers m such that sigma(m)+phi(m) == 2 mod 4.
 * https://oeis.org/A292763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that sigma(m)+phi(m) ≡ 2 (mod 4)
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t termsSize = 0;   /* number of terms stored */
    static size_t termsCap  = 0;   /* allocated capacity */
    static idx_t lastChecked = 0;  /* last m examined */

    /* Produce terms until we have at least n of them */
    while (termsSize < (size_t)n) {
        ++lastChecked;                     /* next candidate m */

        seq_t s  = sigma_of(lastChecked);
        seq_t p  = phi(lastChecked);
        seq_t sum = s + p;

        /* Check if sum ≡ 2 (mod 4) */
        if ((sum & 3u) == 2u) {
            /* Grow array if needed */
            if (termsSize >= termsCap) {
                size_t newCap = termsCap ? termsCap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, newCap * sizeof(seq_t));
                terms = tmp;
                termsCap = newCap;
            }
            terms[termsSize++] = lastChecked;
        }
    }

    return terms[n - 1];
}
