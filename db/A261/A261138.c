/*
 * A261138  The concatenation of 123456...n and the reverse of this number.
 * https://oeis.org/A261138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate(1, 2, …, n, rev(n), rev(n‑1), …, rev(1)),
 *            where rev(x) is the decimal reverse of x preserving the number
 *            of digits of x (e.g. rev(10) = 01). The implementation builds
 *            the number iteratively by multiplying by the appropriate power
 *            of ten and adding the next part
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits_of, rev, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_t seq_pow(seq_t base, idx_t exp); */
#include "../utils/rev.h"       /* idx_t rev(idx_t x); */
#include "../utils/digits_of.h"/* int digits_of(idx_t x); */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* Concatenate 1,2,…,n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t pow10d = seq_pow(10, digits_of(i));
        res = res * pow10d + (seq_t)i;
    }

    /* Concatenate rev(n), rev(n‑1), …, rev(1) */
    for (idx_t i = n; i > 0; --i) {
        seq_t pow10d = seq_pow(10, digits_of(i));
        seq_t r = rev(i);
        res = res * pow10d + r;
    }

    return res;
}
