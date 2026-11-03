/*
 * A292763  Numbers m such that sigma(m)+phi(m) == 2 mod 4.
 * https://oeis.org/A292763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that sigma(m)+phi(m) ≡ 2 (mod 4)
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 88 terms through n = 4698 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

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

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t termsSize = 0;   /* number of terms stored */
    static size_t termsCap  = 0;   /* allocated capacity */
    static idx_t lastChecked = 0;  /* last m examined */

    /* Produce terms until we have at least n of them */
    while (termsSize < (size_t)n) {
        ++lastChecked;                     /* next candidate m */

        seq_t s  = sigma_of(lastChecked);
        seq_t p  = totient(lastChecked);
        seq_t sum = s + p;

        /* Check if sum ≡ 2 (mod 4) */
        if ((sum & 3u) == 2u) {
            /* Grow array if needed */
            if (termsSize >= termsCap) {
                size_t newCap = termsCap ? termsCap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, terms, newCap);
                terms = tmp;
                termsCap = newCap;
            }
            terms[termsSize++] = lastChecked;
        }
    }

    return terms[n - 1];
}
