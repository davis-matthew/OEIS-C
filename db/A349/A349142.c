/*
 * A349142  a(n) = Sum_{d|n} psi(n/d) * A348507(d), where psi is Dedekind psi
 *          (A001615), A348507(n) = A003959(n) - n, and A003959 is fully
 *          multiplicative with a(p) = (p+1).
 * https://oeis.org/A349142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} psi(n/d)* (A003959(d)-d)
 * Range:     u8 n <= 23, u16 n <= 511, u32 n <= 9236, u64 n <= 9236,
 *            u128 n <= 9236, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* maximum exponent that can appear for n <= 2^64 */
#define MAX_EXP 64

/* Recursive helper: generates all divisors of n from its prime factorisation.
 * pr   - array of prime factors (uint64_t)
 * exp  - corresponding exponents (int)
 * idx  - current prime index
 * total- total number of distinct primes
 * curr_d  - product of chosen p^k for processed primes (divisor so far)
 * curr_A  - product of (p+1)^k for the same divisor (A003959(d))
 * curr_psi- product of ψ(n/d) for processed primes
 * sum     - running sum of the Dirichlet convolution
 */
static void dfs(const uint64_t *pr, const int *exp, int idx, int total,
                seq_t curr_d, seq_t curr_A, seq_t curr_psi, seq_t *sum)
{
    if (idx == total) {
        /* Add contribution for this divisor */
        seq_t term = curr_psi * (curr_A - curr_d);
        *sum += term;
        return;
    }

    uint64_t p = pr[idx];
    int e = exp[idx];

    seq_t pow_p[MAX_EXP + 1];
    seq_t pow_p_plus[MAX_EXP + 1];
    seq_t psi_fac[MAX_EXP + 1];

    pow_p[0] = (seq_t)1;
    pow_p_plus[0] = (seq_t)1;
    psi_fac[0] = (seq_t)1;

    for (int k = 1; k <= e; ++k) {
        pow_p[k] = pow_p[k - 1] * (seq_t)p;
        pow_p_plus[k] = pow_p_plus[k - 1] * (seq_t)(p + 1);
    }

    for (int k = 0; k <= e; ++k) {
        int rem = e - k;
        if (rem == 0)
            psi_fac[k] = (seq_t)1;
        else
            psi_fac[k] = pow_p[rem] + pow_p[rem - 1];
    }

    for (int k = 0; k <= e; ++k) {
        dfs(pr, exp, idx + 1, total,
            curr_d * pow_p[k],
            curr_A * pow_p_plus[k],
            curr_psi * psi_fac[k],
            sum);
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* n == 0 is outside the offset; return 0 */
    if (n == 0)
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;

    /* factorisation arrays – safe size for n <= 2^64 */
    uint64_t primes[64];
    int      exps[64];
    int      num = 0;

    uint64_t temp = nn;
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int c = 0;
            while (temp % p == 0) {
                temp /= p;
                ++c;
            }
            primes[num] = p;
            exps[num]   = c;
            ++num;
        }
    }
    if (temp > 1) {
        primes[num] = temp;
        exps[num]   = 1;
        ++num;
    }

    seq_t sum = (seq_t)0;
    dfs(primes, exps, 0, num, (seq_t)1, (seq_t)1, (seq_t)1, &sum);
    return sum;
}
