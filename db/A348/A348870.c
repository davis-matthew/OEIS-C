/*
 * A348870  Interleave A172500 and A172502: The resulting sequence has the
 *          property that the fraction k(n) := a(2*n-1)/a(2*n) has decimal
 *          expansion 0.nnnnnn... .
 * https://oeis.org/A348870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interleave A172500 (reduced numerators) and A172502 (reduced denominators)
   of the fractions n/(10^d−1).  For n = 1,2,…:
      a(2*k-1) = k / gcd(k,10^digits(k)-1)
      a(2*k)   = (10^digits(k)-1) / gcd(k,10^digits(k)-1)
   where digits(k) is the number of decimal digits of k.  Hence
   a(2*k-1)/a(2*k) has decimal expansion 0.k repeating. */
#include "../utils/gcd.h"    /* provides gcd(seq_t,seq_t) */
#include "../utils/pow10.h"  /* provides pow10(idx_t) */

/* Return the number of decimal digits of n (n ≥ 1). */
static idx_t num_digits(idx_t n) {
    idx_t d = 0;
    do { n /= 10; ++d; } while (n);
    return d;
}

seq_t a(idx_t n) {
    if (n & 1) {                   /* odd index – numerator, k = (n+1)/2 */
        idx_t k = (n + 1) >> 1;
        idx_t len = num_digits(k);
        seq_t repunit = pow10(len) - (seq_t)1;
        seq_t g = gcd((seq_t)k, repunit);
        return (seq_t)k / g;
    } else {                       /* even index – denominator, k = n/2 */
        idx_t k = n >> 1;
        idx_t len = num_digits(k);
        seq_t repunit = pow10(len) - (seq_t)1;
        seq_t g = gcd((seq_t)k, repunit);
        return repunit / g;
    }
}
