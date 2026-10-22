/*
 * A385515  Repdigit numbers whose square does not contain the repeated digit.
 * https://oeis.org/A385515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A385515: repdigit numbers whose square does not contain the repeated digit.
 * Sequence definition: for n ≤ 17 the terms are
 *   2,3,4,7,8,9,22,33,44,77,88,333,444,3333,33333,44444,88888
 * for n ≥ 18 the terms form the infinite family of 3‑repeated digits:
 *   a(n) = 33…3  (n-12 digits)
 *   i.e. a(18)=333333, a(19)=3333333, …
 */

#include <stdbool.h>
#include <stddef.h>

/* Generate all repdigits D repeated L times.  (D: 2..9, L:1..) */
static seq_t repdigit(int d, size_t len)
{
    uint64_t v = 0;
    for (size_t i = 0; i < len; ++i)
        v = v * 10 + d;
    return (seq_t)v;
}

/* Test whether the decimal representation of v contains digit d */
static bool contains_digit(uint64_t v, int d)
{
    if (v == 0)
        return d == 0;
    while (v > 0) {
        if ((int)(v % 10) == d)
            return true;
        v /= 10;
    }
    return false;
}

/* Return the n-th term of A385515. */
seq_t a(idx_t n)
{
    /* --------------------------------------------
       Special small terms (first 17) are computed
       once and cached.  They are generated in ascending
       numeric order by increasing length and digit.
       -------------------------------------------- */
    static bool init = false;
    static seq_t small[17];
    static size_t small_cnt = 0;

    if (!init) {
        small_cnt = 0;
        /* We know the largest small term has 5 digits (88888). */
        for (size_t len = 1; small_cnt < 17; ++len) {
            for (int d = 2; d <= 9; ++d) {
                uint64_t val = 0;
                for (size_t i = 0; i < len; ++i)
                    val = val * 10 + d;
                uint64_t sq = val * val;
                /* Reject if the square contains the digit d. */
                if (!contains_digit(sq, d)) {
                    small[small_cnt++] = (seq_t)val;
                    if (small_cnt == 17)
                        break;
                }
            }
        }
        init = true;
    }

    if (n <= small_cnt)
        return small[n - 1];

    /* n ≥ 18: return the 3‑repeated repdigit of length (n-12). */
    size_t len = (size_t)(n - 12);
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i)
        res = res * 10 + 3;
    return res;
}
