/*
 * A328461  a(n) = A276156(n) / A002110(A007814(n)).
 * https://oeis.org/A328461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276156(n) / A002110(A007814(n))
 * Range:     u8 n <= 32, u16 n <= 128, u32 n <= 1024, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* First 65 primes: 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,
 * 103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,
 * 227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,
 * 353,359,367,373,379,383,389,397
 */
static const unsigned int first_primes[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,
    283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397
};
static const size_t prime_count = sizeof(first_primes)/sizeof(first_primes[0]);

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for offset 1, safe fallback */

    /* Determine highest bit position */
    idx_t tmp = n;
    size_t bits = 0;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }
    if (bits == 0) return 0;             /* shouldn't happen for n>=1 */

    size_t maxpos = bits - 1;            /* highest index used in binary expansion */

    if (maxpos >= prime_count) maxpos = prime_count - 1;  /* safety cap */

    /* Build primorials up to maxpos */
    static seq_t primorials[65];
    primorials[0] = 1;
    for (size_t i = 1; i <= maxpos; ++i) {
        primorials[i] = primorials[i-1] * (seq_t)first_primes[i-1];
    }

    /* Compute A276156(n): sum of primorials corresponding to set bits of n */
    seq_t sum = 0;
    idx_t num = n;
    size_t pos = 0;
    while (num) {
        if (num & 1) sum += primorials[pos];
        num >>= 1;
        ++pos;
    }

    /* Find largest primorial index k such that primorials[k] divides sum */
    size_t k;
    for (k = maxpos; k > 0; --k) {
        if (sum % primorials[k] == 0) break;
    }
    if (k == 0 && sum % primorials[0] != 0) return 0;  /* should never occur */

    /* Return quotient */
    return sum / primorials[k];
}
