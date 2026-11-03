/*
 * A351269  Sum of the 6th powers of the squarefree divisors of n.
 * https://oeis.org/A351269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} (1 + p⁶)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1621,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9772 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of the 6th powers of the squarefree divisors of n. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute p⁶ in seq_t, where p is an idx_t. */
static seq_t cube_power_six(idx_t p)
{
    /* Use the fast multiplication pattern: p², p⁴, then p²·p⁴. */
    seq_t p2 = (seq_t)p * (seq_t)p;          /* p² */
    seq_t p4 = p2 * p2;                      /* p⁴ */
    seq_t p6 = p2 * p4;                      /* p⁶ */
    return p6;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Handle factor 2 separately to allow i+=2 optimization. */
    if ((temp & 1) == 0) {
        seq_t term = (seq_t)1 + cube_power_six((idx_t)2);
        result *= term;
        while ((temp & 1) == 0)
            temp >>= 1;  /* temp /= 2; safe for uint64_t */
    }

    /* Trial divide by odd numbers up to √temp. */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            seq_t term = (seq_t)1 + cube_power_six(p);
            result *= term;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If anything remains, it's a prime factor > √original n. */
    if (temp > 1) {
        seq_t term = (seq_t)1 + cube_power_six(temp);
        result *= term;
    }

    return result;
}
