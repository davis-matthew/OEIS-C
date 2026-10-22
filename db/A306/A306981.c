/*
 * A306981  Numbers that can be represented as sum of two fifth powers in two
 *          ways using Gaussian integers generated from Pell numbers.
 * https://oeis.org/A306981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (P(2n)+1)^5 + (P(2n)-1)^5 where P(k) is the k‑th Pell
 *            number (A000129) with offset 0 (P(0)=0, P(1)=1). The function
 *            accepts any idx_t ≥ 1 and returns the n‑th term as a seq_t
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow_seq.h"

/* Cache of Pell numbers: pell[i] = P(i).  P(0)=0, P(1)=1. */
static seq_t *pell = NULL;
static size_t pell_len = 0;   /* number of elements currently stored */
static size_t pell_cap = 0;   /* allocated capacity */

/* Ensure that pell contains entries up to index idx (inclusive). */
static void
ensure_pell(size_t idx)
{
    if (idx + 1 <= pell_cap)
        return;

    size_t new_cap = idx + 1;
    if (pell == NULL) {
        pell = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!pell) abort();
        pell[0] = (seq_t)0;
        pell[1] = (seq_t)1;
        pell_len = 2;
    } else if (new_cap > pell_cap) {
        seq_t *tmp = (seq_t*)realloc(pell, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pell = tmp;
    }
    pell_cap = new_cap;

    for (size_t i = pell_len; i <= idx; ++i) {
        pell[i] = pell[i-1] * (seq_t)2 + pell[i-2];
    }
    pell_len = idx + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n=0, but return 0 */

    /* Pell index needed: P(2n), except that for n==1 the table already
       contains P(3) via the usual recurrence.  The Pell recurrence
       automatically fills any intermediate values. */
    size_t idx = (size_t)(2 * (idx_t)n);
    ensure_pell(idx);

    seq_t p = pell[idx];
    seq_t plus  = p + (seq_t)1;
    seq_t minus = p - (seq_t)1;
    seq_t res = pow_seq(plus, 5) + pow_seq(minus, 5);
    return res;
}
