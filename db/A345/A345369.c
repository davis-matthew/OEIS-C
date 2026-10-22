/*
 * A345369  Lexicographically earliest sequence of distinct positive integers
 *          such that for any n > 0, the product of the first n terms can be
 *          computed without carry in base 10.
 * https://oeis.org/A345369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=1, a(2)=2, a(3)=3.
 * For m ≥ 1: the number 10^m is always in the sequence.
 * If m is a power of two (i.e., m & (m-1) == 0) then 10^m + 1
 * also appears immediately after 10^m.
 * The terms are listed in increasing order.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* handle the first three special terms */
    if (n <= 3) {
        static const seq_t first[3] = { (seq_t)1, (seq_t)2, (seq_t)3 };
        return first[n - 1];
    }

    /* n > 3: the sequence is formed from powers of 10 (and
     * their +1 variants for power‑of‑two exponents).  We
     * walk through the exponents one by one and count the
     * terms until we hit the required index. */
    idx_t k = n - 3;           /* number of terms after the first three */
    idx_t e = 1;               /* current exponent */
    idx_t count = 0;           /* terms counted so far */

    for ( ; ; ++e) {
        /* the term 10^e */
        ++count;
        if (count == k)
            return pow10(e);

        /* if e is a power of two, also include 10^e + 1 */
        if ((e & (e - 1)) == 0) {
            ++count;
            if (count == k)
                return pow10(e) + (seq_t)1;
        }
    }
}
