/*
 * A250037  Numbers n such that m = floor(n/4) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1080, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Runtime: Computes the nth term of A250037 by scanning
   integers and storing all valid terms. */
#include <stdlib.h>
#include <stdbool.h>

/* memoised term list */
static seq_t *terms = NULL;  /* array of terms in ascending order */
static size_t terms_cap = 0; /* allocated capacity of terms[] */
static size_t terms_len = 0; /* number of terms stored so far */
/* last integer examined: holds the greatest integer checked */
static idx_t last_checked = 0;

/* Euclidean algorithm for seq_t (unsigned) */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* binary search: returns true if key is in terms[0 .. terms_len-1] */
static inline bool term_contains(seq_t key)
{
    size_t lo = 0, hi = terms_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (terms[mid] == key) return true;
        if (terms[mid] < key) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* grow the terms array when needed */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (new_terms == NULL) abort();
    terms = new_terms;
    terms_cap = new_cap;
}

/* Extend the memoisation sequence until at least n terms are stored */
static void extend_to(idx_t n)
{
    while (terms_len < n) {
        ++last_checked;                         /* next candidate x */
        seq_t x = (seq_t)last_checked;
        seq_t m = x / (seq_t)4;                 /* floor(x/4) */
        seq_t g = gcd_seq(x, m);
        if (g > (seq_t)1 && (m == (seq_t)0 || term_contains(m))) {
            if (terms_len == terms_cap) grow_terms();
            terms[terms_len++] = x;
        }
    }
}

/* public function required by the build environment */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, return 0 */
    extend_to(n);
    return terms[n - 1];
}
