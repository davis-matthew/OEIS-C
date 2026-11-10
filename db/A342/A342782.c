/*
 * A342782  Positions of words in A342753 having 1st digit 0 and last digit 0.
 * https://oeis.org/A342782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index of the n-th word in A342753's word list that starts
 *            and ends with 0. Word k has first bit (k%3==0), and its chain k
 *            -> child(k) ends at word 1 ("0") or word 3 ("1"). Computed by a
 *            linear scan over word indices
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A342782_child(idx_t k)
{
    idx_t t = (k - 1) / 3;
    return (k % 3 == 0) ? t : (k - 1) - t;
}

static int A342782_ok(idx_t k)
{
    if (k % 3 == 0) return 0;
    for (;;) {
        idx_t c = A342782_child(k);
        if (c == 0) return k == 1;
        k = c;
    }
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    idx_t k = 0;
    while (cnt < n) {
        ++k;
        if (A342782_ok(k)) ++cnt;
    }
    return (seq_t)k;
}
