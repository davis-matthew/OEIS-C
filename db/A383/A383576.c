/*
 * A383576  Characteristic function of numbers of the form k = m^e, where m is
 *          squarefree and (-1)^omega(k) = -mu(e).
 * https://oeis.org/A383576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Text: A383576 – Characteristic function of n = m^e with 
 *  m squarefree, e squarefree, all exponents equal, and parity
 *  condition: parity(omega(m)) != parity(omega(e)). 
 *  a(n) = 1 if these conditions hold, otherwise a(n) = 0. */

/* Required utilities */
#include <stdint.h>
#include <stddef.h>
#include "../utils/mobius.h"
#include "../utils/factor_u64.h"

/* External linkage function as required */
seq_t a(idx_t n)
{
    /* n = 1 is not of the required form */
    if (n <= 1ULL)
        return (seq_t)0;

    /* factor_u64 expects pre‑allocated arrays; use a small static buffer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n, obtaining prime factors and corresponding exponents */
    factor_u64(n, primes, exps, &cnt);

    /* All exponents must be equal; if not, return 0 */
    uint64_t e = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] != e)
            return (seq_t)0;
    }

    /* e must be square‑free: use Möbius function */
    int mu_e = (int)mobius(e);
    if (mu_e == 0)           /* e not square‑free */
        return (seq_t)0;

    /* Parity of omega(e): odd iff mu_e == -1, even otherwise     */
    int parity_e = (mu_e == -1) ? 1 : 0;

    /* Parity of omega(m) is parity of cnt (number of distinct primes) */
    int parity_t = (int)(cnt & 1ULL);

    /* The parity condition: parity_t must differ from parity_e           */
    if (parity_t == (1 - parity_e))
        return (seq_t)1;
    else
        return (seq_t)0;
}
