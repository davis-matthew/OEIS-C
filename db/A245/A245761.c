/*
 * A245761  Numbers with a maximal multiplicative persistence of 1 in any
 *          base.
 * https://oeis.org/A245761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of A245761 by brute force search for numbers with
 * maximal multiplicative persistence of 1 in all bases.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool has_property(seq_t x)
{
    if (x == 0) return true;

    for (seq_t b = 2; b <= x; ++b) {
        seq_t temp = x;
        seq_t prod = 1;
        while (temp > 0) {
            seq_t digit = temp % b;
            prod *= digit;
            if (prod >= b) return false;
            temp /= b;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t term_cnt = 0;
    static size_t term_cap = 0;

    if (term_cnt == 0) {
        term_cap = 32;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
        if (!terms) return 0;          /* allocation failure */

        /* enumerate numbers until we have all known terms (10 in total) */
        seq_t candidate = 0;
        while (term_cnt < 10) {        /* sequence is known to have 10 terms */
            if (has_property(candidate)) {
                if (term_cnt >= term_cap) {
                    term_cap *= 2;
                    terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                    if (!terms) return 0;
                }
                terms[term_cnt++] = candidate;
            }
            ++candidate;
        }
    }

    if (n >= 1 && (idx_t)n <= term_cnt)
        return terms[n - 1];
    return 0;   /* beyond known length of the finite sequence */
}
