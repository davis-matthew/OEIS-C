/*
 * A369031  LCM-transform of permutation induced by partition conjugation via
 *          Heinz numbers (A122111).
 * https://oeis.org/A369031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is prime; a(n) = the k‑th prime when n = p^k (k ≥
 *            1); otherwise a(n) = 1. a(1) = 1
 * Range:     u8 n <= 9443, u16 n <= 9443, u32 n <= 9443, u64 n <= 9443,
 *            u128 n <= 9443, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* List of the first 85 primes – sufficient for the exponent range
   occurring for n up to 10 000 and for 64‑bit indices. */
static const uint64_t _PRIMES[] = {
    2ULL,   3ULL,   5ULL,   7ULL,  11ULL,  13ULL,  17ULL,  19ULL,  23ULL,  29ULL,
   31ULL,  37ULL,  41ULL,  43ULL,  47ULL,  53ULL,  59ULL,  61ULL,  67ULL,  71ULL,
   73ULL,  79ULL,  83ULL,  89ULL,  97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
  127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
  179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL,
  233ULL, 239ULL, 241ULL, 251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL,
  283ULL, 293ULL, 307ULL, 311ULL, 313ULL, 317ULL, 331ULL, 337ULL, 347ULL, 349ULL,
  353ULL, 359ULL, 367ULL, 373ULL, 379ULL, 383ULL, 389ULL, 397ULL, 401ULL, 409ULL,
  419ULL, 421ULL, 431ULL, 433ULL, 439ULL, 443ULL, 449ULL, 457ULL, 461ULL, 463ULL,
  467ULL, 479ULL, 487ULL, 491ULL, 499ULL, 503ULL, 509ULL, 521ULL, 523ULL, 541ULL,
  547ULL, 557ULL, 563ULL, 569ULL, 571ULL, 577ULL, 587ULL, 593ULL, 599ULL, 601ULL,
  607ULL, 613ULL, 617ULL, 619ULL, 631ULL, 641ULL, 643ULL, 647ULL, 653ULL, 659ULL,
  661ULL, 673ULL, 677ULL, 683ULL, 691ULL, 701ULL, 709ULL, 719ULL, 727ULL, 733ULL,
  743ULL, 751ULL, 757ULL, 761ULL, 769ULL, 773ULL, 787ULL, 797ULL, 809ULL, 811ULL
};
static const size_t _PRIMES_LEN = sizeof(_PRIMES) / sizeof(_PRIMES[0]);

/* Return the value of a(n) for the OEIS sequence A369031. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t m = (uint64_t)n;
    uint64_t spf = 0;                     /* smallest prime factor */
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) { spf = d; break; }
    }

    /* m is prime – exponent k = 1 */
    if (spf == 0)
        return (seq_t)2;                   /* the first prime */

    /* factor out the chosen prime */
    uint64_t rem = m;
    unsigned int k = 0;
    while (rem % spf == 0) {
        rem /= spf;
        ++k;
    }

    /* if more than one prime factor, not a prime power */
    if (rem != 1)
        return 1;

    /* n = p^k, return the k‑th prime */
    if (k <= _PRIMES_LEN)
        return (seq_t)_PRIMES[k - 1];

    /* k exceeds the precomputed list – generate primes on the fly.
       k will be very small for all practical indices. */
    unsigned int count = 0;
    uint64_t candidate = 2;
    while (count < k) {
        bool isPrime = true;
        for (uint64_t d = 2; d <= candidate / d; ++d) {
            if (candidate % d == 0) { isPrime = false; break; }
        }
        if (isPrime) {
            ++count;
            if (count == k)
                return (seq_t)candidate;
        }
        ++candidate;
    }

    /* unreachable – return 1 as a safe fallback */
    return 1;
}
