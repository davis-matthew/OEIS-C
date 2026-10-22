/*
 * A391571  a(n) is the value of A280500(n*k, k) for the smallest odd k > 1
 *          for which this value is not zero, where A280500 implements the
 *          carryless base-2 division, and returns 0 if the division leaves
 *          nonzero remainder.
 * https://oeis.org/A391571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is computed by searching for the smallest odd k>1 such that
 *            the carryless base‑2 division of (n*k) by k is exact; the
 *            function returns the quotient from that division
 * Range:     u8 n <= 28, u16 n <= 4009, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the sequence uses carryless base‑2 arithmetic (polynomial
 * division over GF(2)).  A carryless division returns the divisor
 * quotient if the dividend is a multiple of the divisor, otherwise
 * it returns 0.  The function below implements this faithfully. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* carryless division: dividend / divisor over GF(2).
 * Returns the quotient if division is exact, otherwise 0. */
static seq_t carryless_div(seq_t dividend, seq_t divisor)
{
    seq_t rem = dividend;
    seq_t quo = 0;

    /* Fast exit if divisor is zero (should not happen in this problem). */
    if (divisor == 0)
        return 0;

    /* Pre‑compute divisor degree to avoid repeated work. */
    int div_deg = bitlen_seq_t(divisor) - 1;
    if (div_deg < 0)  /* divisor == 0 case */
        return 0;

    /* Long division algorithm over GF(2). */
    while (rem != 0) {
        int rem_deg = bitlen_seq_t(rem) - 1;
        if (rem_deg < div_deg)
            break;   /* remainder degree lower than divisor: division not exact */

        int shift = rem_deg - div_deg;
        quo |= ((seq_t)1 << shift);
        rem ^= (divisor << shift);
    }

    return (rem == 0) ? quo : 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is at least 1, but guard against 0 just in case. */
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;

    /* Search for smallest odd k > 1 with exact carryless division. */
    for (idx_t k = 3; ; k += 2) {
        seq_t prod = sn * (seq_t)k;
        seq_t q = carryless_div(prod, (seq_t)k);
        if (q != 0)
            return q;
        /* The loop is guaranteed to terminate for all n ≥ 1. */
    }
}
