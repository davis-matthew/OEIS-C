/*
 * A306424  Numbers k such that the base-b expansion of k for each b = 3..k-1
 *          never contains more than two distinct digits.
 * https://oeis.org/A306424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the nth integer k such that for all bases b = 3..k-1,
 *            the representation of k in base b contains at most two distinct
 *            digits
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: full, pass, 23 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates the sequence by brute‑force testing of each integer.
   It stores found terms in a static array, generating more only when needed. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Test whether integer k satisfies the two-digit condition for all bases
   3 <= b <= k-1. */
static bool has_at_most_two_digits(seq_t k)
{
    if (k < 3)
        return true;                     /* no basis b in the range */

    /* cast k to idx_t only for loop bounds; k fits in MAX_SCAN anyway */
    idx_t k_lim = (idx_t)k;
    for (idx_t b = 3; b <= k_lim - 1; ++b) {
        seq_t n = k;
        seq_t d1 = (seq_t)-1;            /* sentinel for first distinct digit */
        seq_t d2 = (seq_t)-1;            /* sentinel for second distinct digit */

        while (n > 0) {
            seq_t d = n % (seq_t)b;
            if (d == d1 || d == d2) {
                /* already seen this digit */
            } else if (d1 == (seq_t)-1) {
                d1 = d;
            } else if (d2 == (seq_t)-1) {
                d2 = d;
            } else {                     /* third distinct digit found */
                return false;
            }
            n /= (seq_t)b;
        }
    }
    return true;
}

/* maximum number to scan – small enough to fit into any seq_t (uint8_t…128) and
   large enough to cover the known terms of this sequence (last term 43). */
#define MAX_SCAN 255

/* Sequence generator: a(idx_t n) returns the nth term, offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n = 0 */

    /* static data for memoisation */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t last_k_tested = 0;       /* last value of k that was examined */

    /* generate terms until we have at least n terms or we hit the scan limit */
    while (terms_len < n) {
        ++last_k_tested;
        if (last_k_tested > MAX_SCAN) {   /* no more terms within practical bound */
            return 0;                     /* signal absence of further terms */
        }

        if (has_at_most_two_digits((seq_t)last_k_tested)) {
            if (terms_len == terms_cap) {  /* grow array as needed */
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = (seq_t)last_k_tested;
        }
    }

    return terms[n - 1];
}
