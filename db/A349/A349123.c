/*
 * A349123  a(n) = Sum_{d|n} A038040(n/d) * A003415(d), where A038040(n) =
 *          n*tau(n), and A003415 is the arithmetic derivative of n.
 * https://oeis.org/A349123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A038040(n/d) * A003415(d) A038040(m) = m * τ(m)
 *            A003415(m) = arithmetic derivative of m τ(m) = number of
 *            divisors of m
 * Range:     u8 n <= 23, u16 n <= 479, u32 n <= 9778, u64 n <= 9778,
 *            u128 n <= 9778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* factor(n) fills p[], e[] with the prime factorisation
   and returns the number of distinct prime factors in pc.
   p[i]^e[i] is the i-th prime power in the factorisation. */
static void factor(uint64_t n, uint64_t *p, uint64_t *e, size_t *pc)
{
    *pc = 0;
    if (n < 2) return;
    uint64_t temp = n;
    for (uint64_t f = 2; f <= temp / f; ++f) {
        if (temp % f == 0) {
            uint64_t exp = 0;
            while (temp % f == 0) {
                temp /= f;
                ++exp;
            }
            p[(*pc)] = f;
            e[(*pc)] = exp;
            ++(*pc);
        }
    }
    if (temp > 1) {
        p[(*pc)] = temp;
        e[(*pc)] = 1;
        ++(*pc);
    }
}

/* Recursive enumeration of all divisors of n.
   idx      : current prime index
   cur_d    : current divisor value
   tau_prod : product of (E_i - e_i + 1) for the part processed so far
   cur_exp  : array of the current exponents chosen for each prime
   n        : the original number
   sum      : accumulator for the result */
static void rec_div(size_t idx, uint64_t cur_d, seq_t tau_prod,
                    const uint64_t *p, const uint64_t *e, size_t pc,
                    uint64_t n, seq_t *sum, uint64_t *cur_exp)
{
    if (idx == pc) {
        seq_t d_seq = (seq_t)cur_d;
        seq_t derivative = 0;
        for (size_t i = 0; i < pc; ++i) {
            uint64_t exp = cur_exp[i];
            if (exp > 0) {
                derivative += (d_seq / (seq_t)p[i]) * (seq_t)exp;
            }
        }
        seq_t term = ((seq_t)(n / cur_d)) * tau_prod * derivative;
        *sum += term;
        return;
    }

    uint64_t prime = p[idx];
    uint64_t exp_max = e[idx];
    uint64_t d_local = cur_d;
    for (uint64_t exp = 0; exp <= exp_max; ++exp) {
        cur_exp[idx] = exp;
        seq_t tau_local = tau_prod * (seq_t)(exp_max - exp + 1);
        rec_div(idx + 1, d_local, tau_local, p, e, pc, n, sum, cur_exp);
        if (exp < exp_max) d_local *= prime;
    }
}

/* Public function that computes a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* a(1) = 0, for n < 1 it is undefined */

    uint64_t p[64];
    uint64_t e[64];
    size_t pc;

    factor((uint64_t)n, p, e, &pc);

    seq_t sum = 0;
    uint64_t cur_exp[64];
    rec_div(0, (uint64_t)1, (seq_t)1, p, e, pc, (uint64_t)n, &sum, cur_exp);

    return sum;
}
