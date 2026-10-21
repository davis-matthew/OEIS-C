/*
 * A277128  Values of n such that A019530(n) is different from A007947(n).
 * https://oeis.org/A277128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n‑th positive integer n with rad(n) < max exponent of its
 *            prime factorisation (i.e. A019530(n) ≠ A007947(n))
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Cached list of terms.  The first term (n==1) is 1, by definition. */
static seq_t *term_arr = NULL;
static size_t term_cnt = 0;
static size_t term_cap = 0;
static uint64_t next_to_check = 1;      /* next integer to test */

/* ------------------------------------------------------------------ */
/* Test if a positive integer x belongs to the sequence.  For x>1
   compute the maximum exponent emax in its prime factorisation and
   the radical rad(x).  x is in the sequence iff rad(x) < emax.
   Special‑case 1 is always in the sequence. */
static inline bool in_sequence(uint64_t x)
{
    if (x == 1ULL) return true;   /* 1 is manually included */

    uint64_t tmp = x;
    uint64_t rad = 1ULL;
    int max_exp = 0;

    for (uint64_t p = 2ULL; p * p <= tmp; ++p) {
        if (tmp % p == 0ULL) {
            int exp = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++exp;
            }
            rad *= p;                /* distinct primes only */
            if (exp > max_exp) max_exp = exp;
        }
    }
    if (tmp > 1ULL) {                 /* remaining prime factor */
        rad *= tmp;
        if (1 > max_exp) max_exp = 1;
    }

    /* rad < emax is equivalent to A019530(x) != rad(x) */
    return ((seq_t)rad) < (seq_t)max_exp;
}

/* ------------------------------------------------------------------ */
/* Append a new term to the cached array, growing it as needed. */
static void add_term(seq_t val)
{
    if (term_cnt >= term_cap) {
        term_cap = term_cap ? term_cap * 2 : 1024;
        term_arr = (seq_t *)realloc(term_arr, term_cap * sizeof(seq_t));
    }
    term_arr[term_cnt++] = val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  Builds the list up to the requested index
   (if necessary) and returns the n-th term. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* defensive; offset is 1 */

    while (term_cnt < (size_t)n) {
        if (in_sequence(next_to_check))
            add_term((seq_t)next_to_check);
        ++next_to_check;
    }
    return term_arr[n - 1];
}
