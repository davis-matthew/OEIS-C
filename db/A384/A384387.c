/*
 * A384387  Consecutive states of the linear congruential pseudo-random number
 *          generator (13493037709*s+7261067085) mod 2^35 when started at s=1.
 * https://oeis.org/A384387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (13493037709 * a(n-1) + 7261067085) mod 2^35
 *            for n > 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t LSHIFT35 = ((seq_t)1 << 35);
static const seq_t M_mask   = LSHIFT35 - 1;          /* 2^35 - 1 */

seq_t a(idx_t n)
{
    /* Handle indices less than 1 is undefined; assume caller uses n >= 1. */
    /* Pre‑computed constants modulo 2^35 */
    const seq_t A = (seq_t)13493037709ULL & M_mask;
    const seq_t B = (seq_t)7261067085ULL  & M_mask;

    /* Exponent (n-1) transformations */
    idx_t e = n - 1;

    /* Resulting transformation: mul * x + add (mod 2^35) */
    seq_t mul  = (seq_t)1;   /* identity multiplier */
    seq_t add  = (seq_t)0;   /* identity adder */

    /* Base transformation (apply once) */
    seq_t base_mul = A;
    seq_t base_add = B;

    while (e) {
        if (e & 1) {
            /* Apply base transformation to current result */
            seq_t new_mul = (mul * base_mul) & M_mask;
            seq_t new_add = ((mul * base_add) + add) & M_mask;
            mul = new_mul;
            add = new_add;
        }
        /* Square base transformation: compose it with itself */
        seq_t next_mul = (base_mul * base_mul) & M_mask;
        seq_t next_add = ((base_mul * base_add) + base_add) & M_mask;
        base_mul = next_mul;
        base_add = next_add;

        e >>= 1;
    }

    /* Apply transformation to seed a(1) = 1 */
    return (mul + add) & M_mask;
}
