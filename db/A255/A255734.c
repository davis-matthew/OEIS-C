/*
 * A255734  Numbers n such that A080719(n) divides n.
 * https://oeis.org/A255734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer m such that A080719(m) divides
 *            m * where A080719(m) is obtained by concatenating the binary
 *            representations * of the decimal digits of m and interpreting
 *            the result as a binary number. * * The implementation performs a
 *            sieve‑like search, memoising all terms * foun...
 * Range:     u8 n <= 27, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache of terms that have been found.  It grows as needed. */
static seq_t *terms = NULL;
static size_t terms_capacity = 0;   /* allocated size */
static size_t terms_size = 0;       /* number of terms stored */

/* Next candidate number to test for membership in the sequence. */
static idx_t next_to_test = 1;

/* Number of bits in the binary representation of a decimal digit 0‑9. */
static const unsigned bitlen[10] = {1, 1, 2, 2, 3, 3, 3, 3, 4, 4};

/* Ensure the cache can hold at least {\tt min_cap} terms. */
static void grow_terms(size_t min_cap)
{
    if (min_cap <= terms_capacity)
        return;
    size_t new_cap = terms_capacity ? terms_capacity * 2 : 4;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();
    terms = new_ptr;
    terms_capacity = new_cap;
}

/* Compute A080719(n): concatenate the binary representations of the
 * decimal digits of {\tt n} and interpret the result as a binary number. */
static seq_t A080719(idx_t n)
{
    /* Store digits of n in reverse order.  At most 20 digits for a 64‑bit n. */
    unsigned char digits[20];
    int len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp) {
            digits[len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    seq_t result = 0;
    /* Process digits from most significant to least. */
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        result = (result << bitlen[d]) | (seq_t)d;
    }
    return result;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n of them. */
    while (terms_size < n) {
        idx_t candidate = next_to_test++;
        seq_t f = A080719(candidate);
        /* f == 0 cannot happen for candidate >= 1, but guard anyway. */
        if (f == 0)
            continue;
        seq_t cand_seq = (seq_t)candidate;
        if (cand_seq % f == 0) {
            grow_terms(terms_size + 1);
            terms[terms_size++] = cand_seq;
        }
    }
    return terms[n - 1];
}
