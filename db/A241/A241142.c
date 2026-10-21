/*
 * A241142  Numbers n such that n and 6n share at least one digit.
 * https://oeis.org/A241142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241142: numbers n such that n and 6*n share at least one decimal digit.
   Computed by searching over successive integers. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_candidate = 1;   /* next integer to test */

    if (n == 0) return 0;   /* offset starts at 1 */

    while (terms_len < n) {
        /* Test the next integer */
        next_candidate++;      /* start checking from 2 */

        /* Even numbers always qualify */
        if ((next_candidate & 1ULL) != 0ULL) {  /* odd number */
            seq_t m = (seq_t)next_candidate * 6;
            unsigned int maskN = 0, maskM = 0;
            seq_t temp = next_candidate;
            while (temp) {
                maskN |= 1u << (unsigned)(temp % 10);
                temp /= 10;
            }
            temp = m;
            while (temp) {
                maskM |= 1u << (unsigned)(temp % 10);
                temp /= 10;
            }
            if ((maskN & maskM) == 0)
                continue;   /* not qualifying */
        }

        /* Store the qualifying number */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 128;
            seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) exit(EXIT_FAILURE);
            terms = new_terms;
            terms_cap = new_cap;
        }
        terms[terms_len++] = next_candidate;
    }

    return terms[n - 1];
}
