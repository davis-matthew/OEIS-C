/*
 * A247200  Odd numbers which are neither of the form p*2^m + 1 nor of the
 *          form p*2^m - 1 with p prime.
 * https://oeis.org/A247200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function searches for odd integers that are NOT of the
 * form p*2^m ± 1 where p is prime.  It returns the n‑th such number,
 * indexed with offset 1 (so a(1) = 71). 
 * The search uses the supplied helper isPrimeGeneric(seq_t) to test
 * primality of the odd component after removing all factors of 2.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Return true if x is of the forbidden form p*2^m ± 1 (p prime) */
static bool is_excluded(seq_t x)
{
    seq_t y, v;

    /* Test x - 1 */
    y = x - (seq_t)1;
    if (y != (seq_t)0) {
        /* p = 2 case: x-1 a power of two */
        if ((y & (y - (seq_t)1)) == (seq_t)0)
            return true;
        /* Remove all factors of 2 */
        v = y;
        while ((v & (seq_t)1) == (seq_t)0)
            v >>= 1;
        if (isPrimeGeneric(v))
            return true;
    }

    /* Test x + 1 */
    y = x + (seq_t)1;
    if (y != (seq_t)0) {
        if ((y & (y - (seq_t)1)) == (seq_t)0)
            return true;
        v = y;
        while ((v & (seq_t)1) == (seq_t)0)
            v >>= 1;
        if (isPrimeGeneric(v))
            return true;
    }

    return false;
}

/* Return the n‑th odd number that is not of the forbidden form */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset is 1 – not used */
        return (seq_t)0;

    seq_t cand = (seq_t)1;
    idx_t found = (idx_t)0;

    while (true) {
        if (!is_excluded(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
        cand += (seq_t)2;   /* consider only odd numbers */
    }
}
