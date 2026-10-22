/*
 * A384236  Consecutive states of the linear congruential pseudo-random number
 *          generator (9806*s+1) mod (2^17-1) when started at s=1.
 * https://oeis.org/A384236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9806 * a(n-1) mod (2^17-1)) + 1, a(1)=1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t modpow(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* offset is 1; behaviour for n=0 is irrelevant */
    }
    const seq_t MOD = 131071; /* 2^17 - 1 */
    const seq_t C   = 9806;
    static bool inv_done = false;
    static seq_t inv_c_minus_1;
    if (!inv_done) {
        inv_c_minus_1 = modpow(C - 1, (idx_t)(MOD - 2), MOD); /* Fermat inverse */
        inv_done = true;
    }

    seq_t pow_c = modpow(C, n - 1, MOD);      /* C^(n-1) mod M */
    seq_t numerator = pow_c - 1;              /* (C^(n-1) - 1) mod M */
    seq_t temp = (numerator * inv_c_minus_1) % MOD;
    seq_t r = (C * temp) % MOD;
    return r + 1;                             /* a(n) in [1..M] */
}
