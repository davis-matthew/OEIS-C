/*
 * A343999  a(n) = A011772(n) mod 2, where A011772(n) is the smallest number m
 *          such that m(m+1)/2 is divisible by n.
 * https://oeis.org/A343999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9932, u16 n <= 9932, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A343999:  a(n) = A011772(n) mod 2
   where A011772(n) is the smallest m such that m(m+1)/2 is divisible by n.
   Algorithm: search for the minimal m ≤ 2n satisfying the divisibility,
   return the parity of that m.  The bound 2n is guaranteed to contain
   a solution since m(m+1) contains exactly one factor 2.
*/

seq_t a(idx_t n)
{
    /* Handle the offset: sequence is defined for n ≥ 1.  For safety, if
       n is zero return zero. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Search m from 1 up to 2n-1 inclusive. */
    idx_t limit = n * 2ull;           /* 2n, fits for n up to ~10^4            */
    for (idx_t m = 1; m < limit; ++m) {
        /* Compute the triangular number T(m) = m*(m+1)/2.
           Use 64‑bit arithmetic for the intermediate product to avoid
           overflow for n up to ~10^4.  The result is then reduced modulo n. */
        unsigned long long t = (unsigned long long)m * (m + 1ull) / 2ull;
        if ((t % n) == 0ull) {
            /* Return the parity of the minimal m.  The returned type
               seq_t may be 8/16/32/64/128 bits; the value is always 0 or 1. */
            return (seq_t)(m & 1ull);
        }
    }

    /* The loop invariant guarantees we always return earlier.  The following
       return is for completeness only. */
    return (seq_t)((limit & 1ull) ? 1 : 0);
}
