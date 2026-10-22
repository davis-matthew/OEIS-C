/*
 * A399924  Numbers k such that A007429(k) is odd.
 * https://oeis.org/A399924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_condition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399924: Numbers k such that v(k,2) is even and
   v(k,p) ≡ 0,1 (mod 4) for all odd primes p.
   The function generates members by testing successive integers. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/good_condition.h"

static seq_t *seq = NULL;          /* cached terms */
static idx_t seq_size = 0;         /* number of cached terms */
static size_t seq_cap = 0;         /* capacity of seq array */
static idx_t next_k = 1;           /* next integer to test */

/* Ensure that the sequence buffer has room for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined offset – return 0 */
        return 0;

    while (seq_size < n) {
        if (good_condition(next_k)) {
            ensure_seq_cap(seq_size + 1);
            seq[seq_size] = (seq_t)next_k;
            seq_size++;
        }
        next_k++;
    }
    return seq[n - 1];
}
