/*
 * A368534  a(n) = Sum_{k=1..n} binomial(k+1,2) * n^(n-k).
 * https://oeis.org/A368534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence: A368534
   Formula: a(n) = Sum_{k=1..n} binomial(k+1,2) * n^(n-k)
   Equivalent closed form for n > 1:
     a(n) = n * (2*n^(n+1) - n^3 - n^2 + n - 1) / (2*(n-1)^3)   */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* For small integer types use 128‑bit intermediate to avoid overflow.
       For larger types (arbitrary‑precision) use big_pow and ordinary
       seq_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 n128 = (unsigned __int128)n;

        /* Compute n^(n+1) using 128‑bit integers. */
        unsigned __int128 base = n128;
        unsigned __int128 power = 1;
        unsigned __int128 exp   = (unsigned __int128)(n + 1);
        while (exp) {
            if (exp & 1) power *= base;
            exp >>= 1;
            if (exp) base *= base;
        }

        unsigned __int128 n2    = n128 * n128;
        unsigned __int128 n3    = n2 * n128;
        unsigned __int128 inner = (unsigned __int128)2 * power
                                - n3 - n2 + n128 - (unsigned __int128)1;
        unsigned __int128 numer = n128 * inner;
        unsigned __int128 denom = (unsigned __int128)2 *
                                ((unsigned __int128)(n-1)
                                 * (unsigned __int128)(n-1)
                                 * (unsigned __int128)(n-1));
        unsigned __int128 res   = numer / denom;
        return (seq_t)res;
    } else {
        /* Arbitrary‑precision branch */
        seq_t nt   = (seq_t)n;
        seq_t pow  = big_pow(nt, (seq_t)(n + 1));
        seq_t n2   = nt * nt;
        seq_t n3   = n2 * nt;
        seq_t inner = (seq_t)2 * pow - n3 - n2 + nt - (seq_t)1;
        seq_t numer = nt * inner;
        seq_t denom = (seq_t)2 * ((seq_t)(n-1) * (seq_t)(n-1) * (seq_t)(n-1));
        return numer / denom;
    }
}
