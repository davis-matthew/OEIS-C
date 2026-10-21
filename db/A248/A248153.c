/*
 * A248153  Start with a(0)=10, then a(n) = 7 times the n-th digit of the
 *          sequence.
 * https://oeis.org/A248153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=10; for n>0, a(n)=7*d[n] where d[n] is the n‑th digit of
 *            the concatenated decimal representation of the terms (starting
 *            with the first digit of 10). The sequence is generated lazily
 *            and memoised; any n>=0 can be requested in any order
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;          /* array of terms          */
    static size_t terms_count = 0;       /* number of terms stored  */
    static size_t terms_cap   = 0;       /* capacity of terms array */

    static uint8_t *digits = NULL;       /* concatenated decimal digits */
    static size_t digits_len = 0;        /* number of digits stored    */
    static size_t digits_cap   = 0;      /* capacity of digits array   */

    /* initialise on first call */
    if (terms == NULL) {
        /* allocate initial buffers */
        terms_cap   = 16;
        terms       = (seq_t *)malloc(terms_cap * sizeof(seq_t));

        terms[0]    = (seq_t)10;          /* a(0) = 10   */
        terms_count = 1;

        digits_cap   = 64;
        digits       = (uint8_t *)malloc(digits_cap * sizeof(uint8_t));

        /* digits of a(0) = 10: '1','0'  */
        digits[0]    = 1;
        digits[1]    = 0;
        digits_len   = 2;
    }

    /* extend the sequence up to index n */
    while (terms_count <= (size_t)n) {
        /* ensure term array has room */
        if (terms_count >= terms_cap) {
            size_t new_cap = terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            terms       = tmp;
            terms_cap   = new_cap;
        }

        /* digit d[n] is the (n‑1)-th digit in our 0‑based array */
        uint8_t d = digits[terms_count - 1];
        seq_t next_term = (seq_t)7 * (seq_t)d;
        terms[terms_count] = next_term;
        ++terms_count;

        /* append decimal digits of next_term to digits array */
        /* next_term ∈ [0,63], so it has at most two decimal digits */
        uint64_t val = (uint64_t)next_term;   /* safe: val ≤ 63 */
        if (val < 10) {
            if (digits_len + 1 > digits_cap) {
                size_t new_cap = digits_cap * 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) return (seq_t)0;
                digits = tmp;
                digits_cap = new_cap;
            }
            digits[digits_len++] = (uint8_t)val;
        } else {
            uint8_t second = (uint8_t)(val % 10);
            uint8_t first  = (uint8_t)(val / 10);
            if (digits_len + 2 > digits_cap) {
                size_t new_cap = digits_cap * 2;
                uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
                if (!tmp) return (seq_t)0;
                digits = tmp;
                digits_cap = new_cap;
            }
            digits[digits_len++] = first;
            digits[digits_len++] = second;
        }
    }

    return terms[(size_t)n];
}
