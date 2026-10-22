/*
 * A383685  The lesser of two consecutive primes whose gap equals the
 *          difference between their digital roots.
 * https://oeis.org/A383685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   digit_sum, get_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383685: The lesser of two consecutive primes whose gap equals
   the difference between their digital roots. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"

static seq_t *terms = NULL;        /* stored sequence terms */
static size_t terms_cap = 0;        /* capacity of the terms array */
static size_t terms_len = 0;        /* number of terms currently stored */
static idx_t prime_idx = 1;         /* index of the first prime in the next pair */

/* Compute the digital root of a number as a seq_t value. */
static seq_t digital_root(seq_t x)
{
    const seq_t TEN = 10;
    while (x >= TEN) {
        x = digit_sum(x);       /* repeated digit sum until a single digit */
    }
    return x;
}

/* Append a new term to the terms array, growing it if necessary. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap << 1;
        terms = seq_realloc(terms, terms_cap, new_cap);
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function.  The function may be called in any order;
   it lazily generates terms until the requested index is available. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, n<1 is undefined */

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t p = get_prime(prime_idx);     /* first prime of the pair */
        seq_t q = get_prime(prime_idx + 1); /* second prime of the pair */
        seq_t diff = q - p;                /* prime gap */

        seq_t dr_p = digital_root(p);
        seq_t dr_q = digital_root(q);

        /* Condition: gap equals the difference of digital roots. */
        if (dr_q >= dr_p && (dr_q - dr_p) == diff) {
            append_term(p);                /* store the lesser prime */
        }
        prime_idx++;                       /* move to next overlapping pair */
    }

    return terms[n - 1]; /* return the n‑th term (1‑based indexing) */
}
