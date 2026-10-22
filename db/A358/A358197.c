/*
 * A358197  Numbers k such that 2^k, 5^k and 8^k have the same first digit.
 * https://oeis.org/A358197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A358197.
   The sequence consists of all k such that 2^k, 5^k and 8^k
   have the same first decimal digit.  The offset is 1:
   a(1)=0, a(2)=5, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Scaling factor for integer logarithm arithmetic:
   1e12 gives 12 digits of precision.  All logarithms below are
   multiplied by this factor and rounded down (floor).  */
#define SCALE 1000000000000ULL   /* 1e12 */

/* 10^log10(2), 10^log10(5), 10^log10(8) scaled by SCALE. */
static const unsigned long long LOG2  = 301029995663ULL;   /* log10(2)*SCALE */
static const unsigned long long LOG5  = 698970004336ULL;   /* log10(5)*SCALE */
static const unsigned long long LOG8  = 903089986991ULL;   /* log10(8)*SCALE */

/* Lower and upper bounds (scaled) for each first digit d=1..9.
   If the fractional part of k*log10(b) (scaled) lies in
   [DIGIT_LOWER[d],DIGIT_UPPER[d]), the first digit of b^k is d. */
static const unsigned long long DIGIT_LOWER[10] = {
    0ULL,
    0ULL,                     /* d=1 */
    301029995663ULL,          /* d=2 */
    477121254719ULL,          /* d=3 */
    602059991327ULL,          /* d=4 */
    698970004336ULL,          /* d=5 */
    778151250383ULL,          /* d=6 */
    845098040014ULL,          /* d=7 */
    903089986991ULL,          /* d=8 */
    954242509439ULL           /* d=9 */
};
static const unsigned long long DIGIT_UPPER[10] = {
    0ULL,
    301029995663ULL,          /* d=1 */
    477121254719ULL,          /* d=2 */
    602059991327ULL,          /* d=3 */
    698970004336ULL,          /* d=4 */
    778151250383ULL,          /* d=5 */
    845098040014ULL,          /* d=6 */
    903089986991ULL,          /* d=7 */
    954242509439ULL,          /* d=8 */
    1000000000000ULL          /* d=9 */
};

/* Return the first decimal digit of base^k using scaled integer logs. */
static inline int digit_of_power(idx_t k, unsigned long long log_scaled)
{
    /* Multiply k by the scaled logarithm and keep only the fractional part
       (modulo SCALE).  __int128 is used to avoid overflow for very large k. */
    __int128 prod = (__int128)k * log_scaled;
    unsigned long long frac = (unsigned long long)(prod % SCALE);

    for (int d = 1; d <= 9; ++d) {
        if (frac >= DIGIT_LOWER[d] && frac < DIGIT_UPPER[d]) return d;
    }
    return 0;   /* should never happen */
}

/* Return the n‑th term (1‑based) of A358197. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below the offset */

    idx_t count = 0;   /* number of terms found so far */
    idx_t k = 0;       /* candidate exponent */

    while (true) {
        int d2 = digit_of_power(k, LOG2);
        int d5 = digit_of_power(k, LOG5);
        int d8 = digit_of_power(k, LOG8);
        if (d2 == d5 && d2 == d8) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
