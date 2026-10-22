/*
 * A385039  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (171*s + 11213) mod 53125 when
 *          started at 1.
 * https://oeis.org/A385039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (171*a(n-1)+11213) mod 53125, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t M = (seq_t)53125;
    const seq_t S = (seq_t)171;
    const seq_t C = (seq_t)11213;
    const seq_t INIT = (seq_t)1;

    if (n == 1)
        return INIT;

    idx_t exp = n - 1;
    seq_t res_mul = (seq_t)1;
    seq_t res_add = (seq_t)0;

    seq_t base_mul = S % M;
    seq_t base_add = C % M;

    while (exp)
    {
        if (exp & 1)
        {
            seq_t new_mul = (res_mul * base_mul) % M;
            seq_t new_add = ((res_mul * base_add) + res_add) % M;
            res_mul = new_mul;
            res_add = new_add;
        }

        /* Square the base transform: base ∘ base */
        seq_t new_mul = (base_mul * base_mul) % M;
        seq_t new_add = ((base_mul * base_add) + base_add) % M;
        base_mul = new_mul;
        base_add = new_add;

        exp >>= 1;
    }

    return ((res_mul * INIT) + res_add) % M;
}
