/*
 * A356365  For any nonnegative integer n with binary expansion Sum_{k = 1..w}
 *          2^e_k, let m be the least integer such that the values e_k mod m
 *          are all distinct; a(n) = Sum_{k = 1..w} 2^(e_k mod m).
 * https://oeis.org/A356365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=1..w} 2^(e_k mod m) where n = sum_{k=1..w} 2^e_k,
 *            and m is the least positive integer such that all e_k mod m are
 *            distinct
 * Range:     u8 n <= 270, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Collect the exponents of the set bits in n. */
    idx_t exps[64];          /* maximum of 64 bits for 64‑bit n */
    size_t w = 0;            /* number of set bits */
    idx_t tmp = n;
    idx_t bit = 0;
    while (tmp) {
        if (tmp & 1)
            exps[w++] = bit;
        tmp >>= 1;
        ++bit;
    }

    /* Find the minimal m such that all e_k mod m are distinct.
       We test m = 1,2,3,... until no difference of exponents is
       a multiple of m.  For a single set bit, m = 1 suffices.      */
    idx_t m = 1;
    bool ok;
    for (; ; ++m) {
        ok = true;
        for (size_t i = 0; i + 1 < w && ok; ++i) {
            for (size_t j = i + 1; j < w; ++j) {
                if (((exps[j] - exps[i]) % m) == 0) {
                    ok = false;
                    break;
                }
            }
        }
        if (ok)
            break;   /* found the minimal m */
    }

    /* Compute the sum of 2^(e_k mod m).  The exponent is at most m-1
       (<= maxDiff), which is <= 63, so the shift is safe.            */
    seq_t result = 0;
    for (size_t i = 0; i < w; ++i) {
        idx_t exp = exps[i] % m;
        result += (seq_t)1 << exp;
    }
    return result;
}
