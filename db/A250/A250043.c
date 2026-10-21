/*
 * A250043  Numbers n such that m = floor(n/9) is not coprime to n and, if
 *          nonzero, m is also a term of the sequence.
 * https://oeis.org/A250043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer n such that * m = floor(n/9)
 *            has gcd(n,m) > 1 * and if m > 0, m itself is a term of this
 *            sequence. * This definition is used to generate the sequence
 *            incrementally
 * Range:     u8 n <= 66, u16 n <= 5138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Dynamic cache of the sequence */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static idx_t next_candidate = 2;         /* first integer to test */

/* Ensure capacity for 'need' elements, doubling each time */
static void grow_seq(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) /* out of memory – abort */
        abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Linear search for presence.  seq_len is always sorted. */
static bool contains(seq_t val)
{
    for (size_t i = 0; i < seq_len; ++i)
        if (seq[i] == val)
            return true;
    return false;
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n=0, defensive */

    while (seq_len < (size_t)n) {
        idx_t cand = next_candidate;
        idx_t m_int = cand / 9;          /* floor(n/9) */
        seq_t m = (seq_t)m_int;

        bool ok = false;
        if (m_int == 0) {                /* m == 0: gcd(n,0)=n>1 for n>1 */
            ok = true;
        } else {
            seq_t g = gcd((seq_t)cand, m);
            ok = (g != (seq_t)1) && contains(m);
        }

        if (ok) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)cand;
        }
        ++next_candidate;                /* advance to next integer */
    }
    return seq[n - 1];
}
