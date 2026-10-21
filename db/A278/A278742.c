/*
 * A278742  Lexicographically least strictly increasing sequence such that,
 *          for any n>0, Sum_{k=1..n} a(k) can be computed without carries in
 *          base 10.
 * https://oeis.org/A278742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 42, u64 n <= 82, u128 n <= 165,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278742: Lexicographically least strictly increasing sequence
 * such that for any n>0, Sum_{k=1..n} a(k) can be computed without carries
 * in base 10.
 *
 * The sequence satisfies:
 *   a(n+17) = 10000 * a(n)                           (period 17 in the
 *   a(17k+1) = 10^(4k),  k ≥ 0                        exponent scaling)
 * The first 17 terms are:
 *   1, 2, 3, 10, 11, 12, 20, 30, 100, 110, 200, 300,
 *   1000, 1100, 2000, 2100, 3000
 *
 * Using these properties we can compute a(n) in O(1):
 *   let block = (n-1) / 17
 *   let pos   = (n-1) % 17
 *   a(n) = base[pos] * 10^(4*block)
 *
 * The function below implements this computation using the provided
 * pow10 utility for arbitrary-precision exponents.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Index within the 17-term block (0‑based) and block number (0‑based). */
    idx_t pos   = (n - 1) % 17;
    idx_t block = (n - 1) / 17;

    /* The 17 base values of the sequence. */
    static const seq_t base[17] = {
        1,    /* a(1)  */
        2,    /* a(2)  */
        3,    /* a(3)  */
        10,   /* a(4)  */
        11,   /* a(5)  */
        12,   /* a(6)  */
        20,   /* a(7)  */
        30,   /* a(8)  */
        100,  /* a(9)  */
        110,  /* a(10) */
        200,  /* a(11) */
        300,  /* a(12) */
        1000, /* a(13) */
        1100, /* a(14) */
        2000, /* a(15) */
        2100, /* a(16) */
        3000  /* a(17) */
    };

    /* Compute 10^(4*block). */
    seq_t pow = pow10((idx_t)(4 * block));

    /* Return the scaled base value. */
    return base[pos] * pow;
}
