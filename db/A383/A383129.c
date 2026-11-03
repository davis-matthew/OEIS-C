/*
 * A383129  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (421*s + 54773) mod 259200 when
 *          started at 1.
 * https://oeis.org/A383129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (421*a(n-1) + 54773) mod 259200, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 8523 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the kth power of A modulo M together with the sum of the
   geometric series 1 + A + A^2 + … + A^(k-1) mod M.  The function
   recursively splits k in half to achieve O(log k). */
static inline void power_sum(seq_t A, idx_t k, seq_t *pow_ret, seq_t *sum_ret) {
    const seq_t MOD = (seq_t)259200;
    if (k == 0) {
        *pow_ret = (seq_t)1;
        *sum_ret = (seq_t)0;
        return;
    }
    if ((k & 1) == 0) {                /* k even */
        seq_t pow_half, sum_half;
        power_sum(A, k >> 1, &pow_half, &sum_half);
        seq_t pow = (pow_half * pow_half) % MOD;
        seq_t sum = (sum_half + (pow_half * sum_half) % MOD) % MOD;
        *pow_ret = pow;
        *sum_ret = sum;
    } else {                            /* k odd */
        seq_t pow_prev, sum_prev;
        power_sum(A, k - 1, &pow_prev, &sum_prev);
        seq_t pow = (pow_prev * A) % MOD;
        seq_t sum = (sum_prev + pow_prev) % MOD;
        *pow_ret = pow;
        *sum_ret = sum;
    }
}

/* Main sequence function.  It works for any n >= 1, in any call order,
   and is efficient even for large n because it uses a logarithmic
   exponentiation routine. */
seq_t a(idx_t n) {
    const seq_t MOD   = (seq_t)259200;
    const seq_t A     = (seq_t)421;
    const seq_t C     = (seq_t)54773;
    const seq_t FIRST = (seq_t)1;

    if (n == 1) return FIRST;          /* offset 1 */

    idx_t k = n - 1;                   /* number of steps from the first element */

    seq_t powA, sumSeries;
    power_sum(A, k, &powA, &sumSeries);

    /* a(n) = powA * a(1) + C * sumSeries  (mod MOD) */
    seq_t result = (powA * FIRST + C * sumSeries) % MOD;
    return result;
}
