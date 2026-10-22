/*
 * A369001  a(n) = 1 if n' / gcd(n,n') is even, otherwise 0, where n' stands
 *          for the arithmetic derivative of n, A003415.
 * https://oeis.org/A369001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (n' / gcd(n,n')) is even, otherwise 0 where n' is
 *            the arithmetic derivative of n: n' = Σ_i (n / p_i^e_i) * e_i *
 *            p_i^(e_i-1) for n = ∏ p_i^e_i (derived from n' = n * Σ_i e_i /
 *            p_i)
 * Range:     u8 n <= 657, u16 n <= 8655, u32 n <= 8655, u64 n <= 8655,
 *            u128 n <= 8655, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* power (base)^exp using binary exponentiation; works for any seq_t size */
static seq_t int_pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* arithmetic derivative of n (idx_t), result fits in seq_t */
static seq_t arithmetic_derivative(idx_t n) {
    if (n <= 1) return 0;

    seq_t n_seq = (seq_t)n;
    seq_t result = 0;

    idx_t remaining = n;
    idx_t divisor = 2;
    while (divisor * divisor <= remaining) {
        if (remaining % divisor == 0) {
            idx_t e = 0;
            while (remaining % divisor == 0) {
                remaining /= divisor;
                ++e;
            }
            seq_t p_seq = (seq_t)divisor;
            seq_t pe = int_pow_seq(p_seq, e);          /* p^e       */
            seq_t p_to_e_minus1 = pe / p_seq;          /* p^(e-1)   */
            seq_t term = (n_seq / pe) * (e * p_to_e_minus1);
            result += term;
        }
        /* skip even divisors after 2 */
        divisor += (divisor == 2 ? 1 : 2);
    }
    if (remaining > 1) {                 /* remaining is prime */
        seq_t p_seq = (seq_t)remaining;
        seq_t pe = p_seq;                 /* e = 1 */
        seq_t term = (n_seq / pe) * (1 * (pe / p_seq)); /* (n/p)*1*1 */
        result += term;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* for safety, though n>=1 in calls */
    seq_t derivative = arithmetic_derivative(n);
    seq_t g = gcd((seq_t)n, derivative);
    seq_t t = derivative / g;
    return (t % 2 == 0) ? (seq_t)1 : (seq_t)0;
}
