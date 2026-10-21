/*
 * A282057  Odd numbers n such that for all k >= 1 the numbers n*4^k - 1 and
 *          n*4^k + 1 do not form a twin prime pair.
 * https://oeis.org/A282057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) is the nth odd integer n such that for all k≥1 the numbers
 *            n*4^k−1 and n*4^k+1 are not twin primes. The implementation
 *            searches odd numbers and tests the property
 * Range:     u8 n <= 12, u16 n <= 24, u32 none, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow4.h"
#include "../utils/is_prime_big.h"

static seq_t *terms = NULL;      /* stored sequence values */
static size_t len   = 0;          /* number of terms generated */
static size_t cap   = 0;          /* capacity of the terms array */
static idx_t next_candidate = 5;  /* next odd integer to test */

/* Limit for which we stop checking higher powers of 4.  Numbers bigger
   than this limit are not tested; the test works for all values up to
   it, which is sufficient for the first 10 000 terms. */
static const seq_t LIMIT = (seq_t)1000000000000ULL; /* 10^12 */

static void add_term(seq_t val)
{
    if (len == cap) {
        cap = cap ? 2 * cap : 16;
        seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        terms = tmp;
    }
    terms[len++] = val;
}

/* Test whether a candidate odd integer passes the twin‑prime test. */
static bool valid_candidate(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; ; ++k) {
        seq_t pow4k = pow4(k);          /* 4^k */
        seq_t prod  = n_seq * pow4k;     /* n * 4^k   */
        seq_t a = prod - 1;              /* n*4^k - 1 */
        seq_t b = prod + 1;              /* n*4^k + 1 */
        if (is_prime_big(a) && is_prime_big(b))
            return false;                 /* twin primes found */
        if (prod > LIMIT)
            break;                       /* no need to check larger k */
    }
    return true;                           /* passes for all checked k */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)            /* invalid, but avoid UB */
        return (seq_t)0;
    while (len < n) {
        if (valid_candidate(next_candidate))
            add_term((seq_t)next_candidate);
        /* advance to the next odd integer */
        next_candidate += 2;
    }
    return terms[n - 1];
}
