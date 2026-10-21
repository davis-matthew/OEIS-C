/*
 * A266204  a(n) = G_n(5), where G_n(k) is the Goodstein function defined in
 *          A266201.
 * https://oeis.org/A266204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1124, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Goodstein sequence G_n(5).  For k = 5 the sequence is defined by
   a(0) = 5
   a(n+1) = (hereditary base‑(n+2) shift of a(n)) – 1
   The helper B_m(x) corresponds to: write x in base (m‑1), then
   interpret the same digits in base m.  The algorithm below
   implements this directly for any requested n. */

#include <stdint.h>

static seq_t pow_seq_fixed(seq_t base, seq_t exp)
{
    /* exp is expected to be small; convert to idx_t safely. */
    idx_t e = (idx_t)exp;
    seq_t res = 1;
    while (e) {
        if (e & 1) res *= base;
        e >>= 1;
        if (e) base *= base;
    }
    return res;
}

static seq_t transform_value(seq_t x, idx_t base)
{
    /* Apply one Goodstein transformation: shift all base‑occurences
       from 'base' to 'base+1' in the hereditary representation of x. */
    if (x < (seq_t)base) return x;

    seq_t result = 0;
    seq_t curr = x;
    idx_t pos = 0;

    while (curr > 0) {
        seq_t digit = curr % base;
        if (digit != 0) {
            seq_t exp_shift = transform_value((seq_t)pos, base);
            seq_t weight    = pow_seq_fixed((seq_t)(base + 1), exp_shift);
            result += digit * weight;
        }
        curr /= base;
        ++pos;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = 5;          /* G_0(5) */
    idx_t  base = 2;      /* current base for representation */

    for (idx_t i = 0; i < n; ++i) {
        x = transform_value(x, base);
        --x;               /* subtract 1 */
        ++base;
    }
    return x;
}
