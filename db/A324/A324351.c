/*
 * A324351  Square array read by antidiagonals: A(x,y) is the result from
 *          writing x and y in primorial base (A049345) and starting from
 *          their least significant ends, always choosing a minimal digit from
 *          each digit position, and converting back to decimal.
 * https://oeis.org/A324351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A324351.
   The array entry A(x,y) is obtained by writing x and y in primorial
   base (A049345), taking the minimal digit at each position (starting
   from the least‑significant ends), and interpreting the resulting
   primorial digits back to decimal.  The sequence a(n) is read from
   the square array A(x,y) by antidiagonals (k = x+y). */

#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* First 200 primes – enough for all positions needed up to the limits
   used in the tests. */
static const unsigned int primes[200] = {
     2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97,101,103,107,109,113,
   127,131,137,139,149,151,157,163,167,173,
   179,181,191,193,197,199,211,223,227,229,
   233,239,241,251,257,263,269,271,277,281,
   283,293,307,311,313,317,331,337,347,349,
   353,359,367,373,379,383,389,397,401,409,
   419,421,431,433,439,443,449,457,461,463,
   467,479,487,491,499,503,509,521,523,541,
   547,557,563,569,571,577,587,593,599,601,
   607,613,617,619,631,641,643,647,653,659,
   661,673,677,683,691,701,709,719,727,733,
   739,743,751,757,761,769,773,787,797,809,
   811,821,823,827,829,839,853,857,859,863,
   877,881,883,887,907,911,919,929,937,941,
   947,953,967,971,977,983,991,997,1009,1013
};
static const size_t PRIMES_COUNT = sizeof(primes) / sizeof(primes[0]);

static inline unsigned long long triangular(unsigned long long k)
{
    return k * (k + 1) / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;

    /* Find the diagonal k such that triangular(k) <= n < triangular(k+1) */
    unsigned long long k = (unsigned long long)((sqrt(8.0L * nn + 1.0L) - 1.0L) / 2.0L);
    while (triangular(k + 1) <= nn) ++k;
    while (triangular(k) > nn) --k;

    unsigned long long offset = nn - triangular(k);
    unsigned long long x = offset;
    unsigned long long y = k - offset;

    /* Obtain primorial digits of x and y (least significant first) */
    uint8_t dx[200] = {0};
    uint8_t dy[200] = {0};
    size_t lenx = 0, leny = 0;
    unsigned long long t;

    t = x;
    size_t i = 0;
    while (t > 0 && i < PRIMES_COUNT) {
        dx[lenx++] = t % primes[i];
        t /= primes[i];
        ++i;
    }

    t = y;
    i = 0;
    while (t > 0 && i < PRIMES_COUNT) {
        dy[leny++] = t % primes[i];
        t /= primes[i];
        ++i;
    }

    /* Build minimal-digit primorial representation and convert to decimal */
    size_t maxlen = lenx > leny ? lenx : leny;
    seq_t result = 0;
    seq_t weight = 1;          /* product of primes of all lower positions */

    for (size_t pos = 0; pos < maxlen; ++pos) {
        uint8_t d1 = (pos < lenx) ? dx[pos] : 0;
        uint8_t d2 = (pos < leny) ? dy[pos] : 0;
        uint8_t dmin = (d1 < d2) ? d1 : d2;

        result += (seq_t)dmin * weight;
        weight *= (seq_t)primes[pos];
    }

    return result;
}
