/*
 * A384567  Consecutive states of the linear congruential pseudo-random number
 *          generator for the Atari ST when started at 1.
 * https://oeis.org/A384567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3141592621 * a(n-1) + 1) mod 2^32, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1, but safe to return 0 */
    if (n == 1) return 1;   /* the first term */

    /* constants in the sequence */
    const seq_t MULT = (seq_t)3141592621u;      /* multiplier */
    const seq_t ONE  = (seq_t)1;                /* increment */
    const seq_t MASK = (seq_t)0xFFFFFFFFu;      /* lower 32 bits */

    idx_t k = n - 1;           /* exponent */

    /* result transformation: result_mul*x + result_add */
    seq_t result_mul = (seq_t)1; /* identity mul */
    seq_t result_add = (seq_t)0; /* identity add */

    /* base transformation: base_mul*x + base_add  (corresponds to one step of the RNG) */
    seq_t base_mul = MULT;
    seq_t base_add = ONE;

    while (k) {
        if (k & 1) {
            /* result = result * base  (apply base after result) */
            seq_t tmp_mul = (result_mul * base_mul) & MASK;
            seq_t tmp_add = (result_mul * base_add + result_add) & MASK;
            result_mul = tmp_mul;
            result_add = tmp_add;
        }

        /* base = base * base  (square the transformation) */
        seq_t tmp_mul = (base_mul * base_mul) & MASK;
        seq_t tmp_add = (base_mul * base_add + base_add) & MASK;
        base_mul = tmp_mul;
        base_add = tmp_add;

        k >>= 1;
    }

    /* a(n) = result_mul * a(1) + result_add = result_mul + result_add (mod 2^32) */
    return (result_mul + result_add) & MASK;
}
