/*
 * A274172  Nonsquare composites with all prime factors larger than 5.
 * https://oeis.org/A274172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th nonsquare composite that is not divisible by
 *            2,3,5 (equivalent to a composite number whose prime factors are
 *            all > 5)
 * Range:     u8 n <= 3, u16 n <= 1787, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, isPrimeTrial, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/sqrt_floor.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but harmless */

    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t cur = 6;        /* value to be tested next */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
    }

    /* Generate terms until we have at least n */
    while (terms_len < n) {
        /* Skip numbers divisible by 2, 3 or 5 */
        if ((cur % 2 != 0) && (cur % 3 != 0) && (cur % 5 != 0)) {
            /* Must be composite */
            if (!isPrimeTrial(cur)) {
                /* Must not be a perfect square */
                seq_t r = sqrt_floor(cur);
                if (!(r * r == cur)) {
                    /* Store the term */
                    if (terms_cap <= terms_len) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                        while (new_cap <= terms_len) new_cap *= 2;
                        grow_seq(&terms, &terms_cap, new_cap);
                    }
                    terms[terms_len++] = cur;
                }
            }
        }
        cur++;   /* advance to next value to examine */
    }

    return terms[n - 1];
}
