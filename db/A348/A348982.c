/*
 * A348982  a(n) = Sum_{d|n} psi(n/d) * A322582(d), where psi is Dedekind psi
 *          (A001615), A322582(n) = n - A003958(n), and A003958 is fully
 *          multiplicative with a(p) = (p-1).
 * https://oeis.org/A348982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} psi(n/d) * ( d - product_{p|d} (p-1)^{exp(d,p)}
 *            )
 * Range:     u8 n <= 33, u16 n <= 1439, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 94 terms through n = 9434 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where psi(m) = Dedekind psi = m * prod_{p|m} (1+1/p) */
/* This implements the Dirichlet convolution of A001615 and A322582. */

#include <stdint.h>
#include <stddef.h>

/* Internal recursive helper that accumulates the sum. */
static void _rec(idx_t n,
                 const uint64_t *primes,
                 const size_t *exps,
                 size_t pcnt,
                 size_t depth,
                 size_t *cho_exp,
                 seq_t cur_val,
                 seq_t prod_minus1,
                 seq_t *sum)
{
    if (depth == pcnt) {
        /* At leaf: cur_val is the divisor d. */
        seq_t d_val = cur_val;
        seq_t A322 = d_val - prod_minus1;          /* d - product(p-1)^k */

        idx_t m_int = n / (idx_t)d_val;              /* n/d */
        seq_t psi = (seq_t)m_int;                   /* start with m */

        /* compute the product (1+1/p) for each prime where exponent in m>0 */
        for (size_t j = 0; j < pcnt; ++j) {
            size_t exp_m = exps[j] - cho_exp[j];
            if (exp_m > 0) {
                psi = psi * (seq_t)(primes[j] + 1);
                psi = psi / primes[j];
            }
        }

        seq_t contrib = psi * A322;
        *sum += contrib;
        return;
    }

    uint64_t p = primes[depth];
    size_t maxe = exps[depth];
    seq_t val = cur_val;
    seq_t prod = prod_minus1;

    for (size_t e = 0; e <= maxe; ++e) {
        cho_exp[depth] = e;
        _rec(n, primes, exps, pcnt, depth + 1, cho_exp, val, prod, sum);
        if (e < maxe) {
            val = val * (seq_t)p;
            prod = prod * (seq_t)(p - 1);
        }
    }
}

/* Forward declaration of the function to be implemented. */
seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Factorise n */
    uint64_t primes[64];
    size_t exps[64];
    size_t pcnt = 0;

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;
        exps[pcnt] = 1;
        ++pcnt;
    }

    size_t cho_exp[64];   /* storage for current exponent choices, 64 > max possible primes for 64‑bit n */
    seq_t sum = 0;

    _rec(n, primes, exps, pcnt, 0, cho_exp, (seq_t)1, (seq_t)1, &sum);

    return sum;
}
