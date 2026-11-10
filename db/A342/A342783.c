/*
 * A342783  Positions of words in A342753 having 1st digit 0 and last digit 1.
 * https://oeis.org/A342783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m with 3 !| m whose word in A342753 ends in
 *            1. Word chain: m -> c(m), c(m)=(m-1)/3 if 3|m else
 *            (m-1)-(m-1)/3; chain ends at 1 ("0") or 3 ("1"). Computed by a
 *            sieve-like scan with memoized terminal values
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A342783_end3 = NULL;   /* 1 if chain of m ends at 3 */
static idx_t *A342783_res = NULL;
static size_t A342783_cnt = 0, A342783_rescap = 0, A342783_cap = 0;
static idx_t A342783_last = 0;

static idx_t A342783_child(idx_t m)
{
    idx_t a = m - 1;
    idx_t t = a / 3;
    return (m % 3 == 0) ? t : a - t;
}

seq_t a(idx_t n)
{
    while (A342783_cnt < (size_t)n) {
        idx_t m = A342783_last + 1;
        if ((size_t)m >= A342783_cap) {
            size_t nc = A342783_cap ? A342783_cap * 2 : 1024;
            unsigned char *tmp = (unsigned char *)realloc(A342783_end3, nc);
            if (!tmp) exit(EXIT_FAILURE);
            A342783_end3 = tmp;
            A342783_cap = nc;
        }
        if (m == 1) A342783_end3[m] = 0;
        else if (m == 3) A342783_end3[m] = 1;
        else A342783_end3[m] = A342783_end3[A342783_child(m)];
        A342783_last = m;
        if (m % 3 != 0 && A342783_end3[m]) {
            if (A342783_cnt >= A342783_rescap) {
                size_t nr = A342783_rescap ? A342783_rescap * 2 : 256;
                idx_t *t2 = (idx_t *)realloc(A342783_res, nr * sizeof(idx_t));
                if (!t2) exit(EXIT_FAILURE);
                A342783_res = t2;
                A342783_rescap = nr;
            }
            A342783_res[A342783_cnt++] = m;
        }
    }
    return (seq_t)A342783_res[n - 1];
}
