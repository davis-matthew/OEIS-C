/*
 * A392520  Numbers k such that sigma(k) = psi(k) + tau(k)^2 + omega(k)^3.
 * https://oeis.org/A392520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   Numbers k such that sigma(k) = psi(k) + tau(k)^2 + omega(k)^3.
 *            Implemented by brute‐force search: test each integer k in
 *            increasing order, compute sigma, psi, tau, omega and store those
 *            satisfying the equality. The sequence is generated lazily and
 *            cached so that calls may come in any order
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/factor_u64.h"

/* helper: fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, size_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t terms_len = 0, terms_cap = 0;
    static idx_t candidate = 1;          /* start from 1 */

    /* Ensure we have computed at least n terms. */
    if (terms_len < n) {
        /* initial allocation */
        if (terms_cap == 0) {
            terms_cap = 64;
            terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        }

        /* loop until we find enough terms */
        while (terms_len < n) {
            uint64_t u = (uint64_t)candidate;   /* cast for factor_u64 */
            /* compute sigma */
            seq_t sigma = sigma_of(u);

            /* factorization */
            uint64_t primes[64], exps[64];
            size_t cnt;
            factor_u64(u, primes, exps, &cnt);

            /* compute psi = Π (p^e + p^(e-1)) */
            seq_t psi = 1;
            for (size_t i = 0; i < cnt; ++i) {
                seq_t p  = (seq_t)primes[i];
                seq_t pe = pow_seq(p, exps[i]);          /* p^e */
                seq_t pe1 = pow_seq(p, exps[i] - 1);     /* p^(e-1) */
                seq_t term = pe + pe1;
                psi *= term;
            }

            /* tau and omega */
            seq_t tau   = (seq_t)divisor_count_u64(u);
            seq_t omega = (seq_t)cnt;

            /* RHS: psi + tau^2 + omega^3 */
            seq_t rhs = psi + tau * tau + pow_seq(omega, 3);

            /* equality test */
            if (sigma == rhs) {
                if (terms_len == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)candidate;
            }

            ++candidate;
        }
    }

    return terms[n - 1];
}
