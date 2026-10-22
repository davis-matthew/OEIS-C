/*
 * A323364  Sum of Dedekind's psi, A001615, and its Dirichlet inverse,
 *          A323363.
 * https://oeis.org/A323364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ψ(n) + g(n) * where ψ(n) = n ∏_{p|n} (1+1/p) (Dedekind's
 *            psi function) * and g(n) is the Dirichlet inverse of ψ,
 *            multiplicative with * g(p^k) = (-1)^k (p+1) (k ≥ 1), g(1) = 1. *
 *            Thus for n = ∏ p^k, * ψ(n) = ∏ p^{k-1} (p+1), * g(n) = (∏ (p+1))
 *            · (-1)^{∑k}. * The function below computes a(n)...
 * Range:     u8 n <= 83, u16 n <= 9733, u32 n <= 9733, u64 n <= 9733,
 *            u128 n <= 9733, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    idx_t tmp = n;                  /* local copy for factorisation */
    seq_t prod_pp1 = 1;             /* product of (p+1) over distinct primes */
    seq_t prod_p_pow = 1;           /* product of p^{k-1} for each prime power */
    idx_t omega = 0;                /* sum of exponents, used for sign of g */

    /* simple trial division */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p != 0) continue;

        idx_t exp = 0;
        seq_t p_pow = 1;            /* p^(exp-1) , start with 1 */

        /* factor out p */
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
            if (exp > 1)             /* multiply only when exponent > 1 */
                p_pow *= (seq_t)p;   /* p^(exp-1) accumulates */
        }

        prod_pp1 *= (seq_t)(p + 1);  /* (p+1) factor */
        prod_p_pow *= p_pow;        /* p^{k-1} factor */
        omega += exp;               /* exponent contribution to Ω(n)   */
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1) {
        prod_pp1 *= (seq_t)(tmp + 1);
        /* p^{k-1} for k=1 is 1, so prod_p_pow unchanged */
        omega += 1;
    }

    /* compute ψ(n) */
    seq_t psi = prod_p_pow * prod_pp1;

    /* a(n) = ψ(n) + g(n) ; use sign to avoid intermediate negative */
    seq_t result;
    if (omega & 1U) {
        /* odd total exponent → g(n) = -∏(p+1) */
        result = psi - prod_pp1;
    } else {
        /* even total exponent → g(n) = +∏(p+1) */
        result = psi + prod_pp1;
    }

    return result;
}
