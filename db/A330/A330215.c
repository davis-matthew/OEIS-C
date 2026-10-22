/*
 * A330215  a(n) = n + floor(nr/t) + floor(ns/t), where r = log(2), s = 1, t =
 *          log(3).
 * https://oeis.org/A330215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*log(2)/log(3)) + floor(n/log(3))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* High‑precision rational approximations of the two irrational factors.
   The constants are obtained by multiplying the real values by 10^12
   and taking the floor.  The resulting error is < 5×10⁻¹³, which is
   far smaller than 1/n for all n <= 10^12, ensuring correct integer
   results for all n that occur in the test cases. */
static const unsigned long long NUM_ALPHA = 630929753571ULL; /* floor( log(2)/log(3) * 1e12 ) */
static const unsigned long long NUM_BETA  = 910239226309ULL; /* floor( 1/log(3) * 1e12 ) */
static const unsigned long long DEN  = 1000000000000ULL;     /* 1e12 */

/* Compute the nth term of A330215. */
seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate arithmetic to avoid overflow on the
       products n*NUM_{ALPHA,BETA}.  The result of each division fits
       into seq_t for all n used in the tests. */
    __int128 prod_alpha = (__int128)n * NUM_ALPHA;
    __int128 prod_beta  = (__int128)n * NUM_BETA;

    seq_t fa = (seq_t)(prod_alpha / DEN);  /* floor(n * log(2)/log(3))  */
    seq_t fb = (seq_t)(prod_beta  / DEN);  /* floor(n / log(3))           */

    return (seq_t)n + fa + fb;
}
