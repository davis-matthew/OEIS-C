/*
 * A363827  Highest power of 2 dividing n which is <= sqrt(n).
 * https://oeis.org/A363827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^min(v2(n), floor_log2(floor(sqrt(n))))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the highest power of 2 dividing n which is <= sqrt(n).
   Uses only integer arithmetic and the C/C++ built‑in integer
   functions.  For seq_t that cannot contain the result the function
   may overflow, but all values that fit in seq_t are correct. */

#include <stdint.h>
#include <stdbool.h>

/* fast integer square root of an unsigned 64‑bit value */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t res = 0;
    idx_t bit = ((idx_t)1) << 62;        /* highest possible square bit */
    /* bring bit down to a value <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        idx_t tmp = res + bit;
        if (tmp <= n / tmp) {
            res = tmp;
        }
        bit >>= 1;
    }
    return res;
}

/* floor of logarithm base 2 of a 64‑bit value (v > 0) */
static unsigned floor_log2_u64(idx_t v)
{
    return 63 - __builtin_clzll(v);      /* count leading zeros */
}

seq_t a(idx_t n)
{
    /* exponent of 2 dividing n */
    unsigned e1 = __builtin_ctzll(n);

    /* integer part of sqrt(n) */
    idx_t sf = isqrt_u64(n);

    /* exponent of the largest power of 2 <= sqrt(n) */
    unsigned e2 = floor_log2_u64(sf);

    /* take the smaller exponent */
    unsigned e = e1 < e2 ? e1 : e2;

    /* 2^e as seq_t */
    return ((seq_t)1 << e);
}
