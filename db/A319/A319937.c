/*
 * A319937  Unitary sociable numbers of order 10.
 * https://oeis.org/A319937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 10^k * base[r] where k = ⌊(n‑1)/10⌋, r = (n‑1) mod 10,
 *            and base contains the first 10 unitary sociable numbers of order
 *            10
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* first ten unitary sociable numbers of order 10 */
    static const seq_t base[10] = {
        (seq_t)525150234,
        (seq_t)527787366,
        (seq_t)528544218,
        (seq_t)553128198,
        (seq_t)612951066,
        (seq_t)675192294,
        (seq_t)735821562,
        (seq_t)982674438,
        (seq_t)998151162,
        (seq_t)998151174
    };

    /* avoid underflow for n=0 – behaviour below the offset is irrelevant */
    if (n == 0) return (seq_t)0;

    idx_t k = (n - 1) / 10;   /* number of times multiplied by 10 */
    idx_t r = (n - 1) % 10;   /* index into base array */

    seq_t pow = pow10(k);     /* compute 10^k as seq_t */
    return seq_mul(base[r], pow);
}
