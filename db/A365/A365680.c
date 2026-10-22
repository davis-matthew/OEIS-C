/*
 * A365680  The number of exponentially squarefree divisors of n.
 * https://oeis.org/A365680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A365680: Number of exponentially squarefree divisors of n.
   A(n) is multiplicative with A(p^e) = sqf(e)+1, where sqf(e) is the
   number of squarefree integers ≤ e.  The function below computes A(n)
   for every n ≥ 1.  It uses the provided factor_u64 and mu_of utilities. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/mu_of.h"

/* Count squarefree integers in [1, E] using the Möbius function. */
static seq_t
sqf_count(idx_t E)
{
    seq_t sum = 0;
    for (idx_t k = 1; k * k <= E; ++k) {
        seq_t mu_seq = mu_of(k);
        int mu = (int)mu_seq;          /* mu ∈ {−1,0,1} */
        if (mu == 0)
            continue;
        idx_t term = E / (k * k);
        if (mu > 0)
            sum += (seq_t)term;
        else
            sum -= (seq_t)term;
    }
    return sum;
}

seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                /* a(1) = 1 */

    /* Factor n; exponents are stored in ex[ ] (uint64_t). */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)ex[i];        /* exponent e ≥ 1 */
        seq_t part = sqf_count(e) + (seq_t)1; /* A(p^e) */
        result *= part;
    }

    return result;
}
