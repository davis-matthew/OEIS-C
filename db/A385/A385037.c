/*
 * A385037  Consecutive states of the linear congruential pseudo-random number
 *          generator (936*s + 1399) mod 6655 when started at s=1.
 * https://oeis.org/A385037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(1) = 1; a(n) = (936 * a(n-1) + 1399) mod 6655
 * Range:     u8 n <= 1, u16 n <= 6655, u32 n <= 6655, u64 n <= 6655,
 *            u128 n <= 6655, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const seq_t MOD = 6655;
    const seq_t A1  = 1;
    const seq_t INIT_MUL = 936;
    const seq_t INIT_ADD = 1399;

    /* Handle the offset: the sequence is defined for n >= 1.  */
    if (n == 0) {
        return 0;
    }

    seq_t mul  = 1;          /* multiplier of the current transform */
    seq_t add  = 0;          /* additive constant of the current transform */
    seq_t pmul = INIT_MUL;   /* multiplier of the power transform    */
    seq_t padd = INIT_ADD;   /* additive constant of the power transform */
    idx_t k = n - 1;         /* we need to apply the transform k times */

    while (k > 0) {
        if (k & 1) {
            /* Compose (mul,add) with (pmul,padd) */
            seq_t new_mul = (pmul * mul) % MOD;
            seq_t new_add = (pmul * add + padd) % MOD;
            mul = new_mul;
            add = new_add;
        }

        /* Square the power transform: (pmul,padd) = (pmul,padd) ∘ (pmul,padd) */
        seq_t new_pmul = (pmul * pmul) % MOD;
        seq_t new_padd = (pmul * padd + padd) % MOD;
        pmul = new_pmul;
        padd = new_padd;

        k >>= 1;
    }

    /* Apply the resulting transform to the initial value A1 */
    seq_t result = (mul * A1 + add) % MOD;
    return result;
}
