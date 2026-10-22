/*
 * A373002  a(n) = Sum_{k=1..n} tau( (n/gcd(k,n))^n ).
 * https://oeis.org/A373002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * tau(d^n)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the auxiliary recursive routine dfs_divisors to enumerate all
 * divisors d of n, compute phi(d) and tau(d^n), and accumulate the sum
 * in seq_t.  The implementation works for any seq_t and idx_t
 * provided by the build system. */

#include <stddef.h>
#include <stdint.h>   /* for uint64_t used in the factorisation */

/* Recursive divisor enumeration.
 * Parameters:
 *   primes   : array of prime factors of n
 *   exps     : corresponding exponents in the prime factorisation
 *   pcnt     : number of distinct primes
 *   idx      : current recursion depth (prime index)
 *   cur_d    : divisor constructed so far
 *   cur_exp  : array holding exponents of the current divisor
 *   n        : original number (as idx_t)
 *   total    : pointer to the running sum (seq_t) */
static void dfs_divisors(const uint64_t *primes, const int *exps, size_t pcnt,
                         size_t idx, seq_t cur_d, const int *cur_exp,
                         idx_t n, seq_t *total)
{
    if (idx == pcnt) {
        /* leaf: compute phi(cur_d) and tau(cur_d^n) */
        seq_t phi = cur_d;
        seq_t tau = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            if (cur_exp[i] != 0) {
                /* phi(d) *= (1 - 1/p) */
                phi -= phi / primes[i];
                /* tau(d^n) *= (exponent * n + 1) */
                seq_t term = (seq_t)cur_exp[i] * (seq_t)n + 1;
                tau *= term;
            }
        }
        *total += phi * tau;
        return;
    }

    uint64_t p = primes[idx];
    /* iterate exponent of current prime from 0 to its maximum */
    for (int e = 0; e <= exps[idx]; ++e) {
        const int *cur_exp_ptr = cur_exp;   /* keep compiler happy */
        int exp_store[16];                  /* local copy to modify */
        /* copy current exponents and set the current one */
        for (size_t j = 0; j < pcnt; ++j) exp_store[j] = cur_exp[j];
        exp_store[idx] = e;

        seq_t d2 = cur_d;
        for (int k = 0; k < e; ++k) d2 *= p;

        dfs_divisors(primes, exps, pcnt, idx + 1, d2,
                     exp_store, n, total);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but harmless */

    /* Factorise n */
    uint64_t num = (uint64_t)n;
    uint64_t primes[16];
    int exps[16];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= num; ++p) {
        if (num % p == 0) {
            int e = 0;
            while (num % p == 0) {
                num /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (num > 1) {
        primes[pcnt] = num;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    int cur_exp[16] = {0};
    seq_t total = 0;
    dfs_divisors(primes, exps, pcnt, 0, (seq_t)1,
                 cur_exp, n, &total);
    return total;
}
