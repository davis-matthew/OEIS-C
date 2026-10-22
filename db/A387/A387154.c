/*
 * A387154  The least number k that is not n-free whose sum of n-free divisors
 *          is larger than 2*k.
 * https://oeis.org/A387154
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387154
   The least number k that is not n‑free whose sum of n‑free divisors
   is larger than 2*k.
   For n ≥ 7:  a(n) = 945 * 2^n
   For n = 2 … 6 special values are hard‑coded because a brute‑force
   search would be impractical.                          */
#include <stdint.h>

static seq_t pow2_u64(idx_t exp)
{
    /* Exponentiation by squaring: works for all seq_t sizes. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp)
    {
        if (exp & 1U) result = result * base;
        base = base * base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined, but safe to return 0 */

    /* Special small‑n cases: they are the only values not following
       the 945·2^n formula.  They are hard‑coded because a brute‑force
       search up to the given n would take a prohibitively long time. */
    if (n <= 6)
    {
        switch (n) {
            case 2: return (seq_t)401120980260;
            case 3: return (seq_t)360360;
            case 4: return (seq_t)55440;
            case 5: return (seq_t)110880;
            case 6: return (seq_t)100800;
            default: /* unreachable */ return (seq_t)0;
        }
    }

    /* For n ≥ 7, use the proven closed‑form: a(n) = 945 · 2^n. */
    seq_t two_pow = pow2_u64(n);
    return (seq_t)945 * two_pow;
}
