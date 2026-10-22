/*
 * A342915  a(n) = gcd(1+n, A001615(n)), where A001615 is Dedekind psi, n *
 *          Product_{p|n, p prime} (1 + 1/p).
 * https://oeis.org/A342915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(1+n, A001615(n))
 * Range:     u8 n <= 256, u16 n <= 9403, u32 n <= 9403, u64 n <= 9403,
 *            u128 n <= 9403, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001615(n) is Dedekind psi: n * Π_{p|n} (1 + 1/p) */

/* This function computes a(n) = gcd(1+n, ψ(n))
 * by factoring n via simple trial division and
 * building ψ(n) multiplicatively.  The gcd helper
 * is taken from the utils/gcd.h header. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Convert index to 64-bit value for arithmetic */
    uint64_t tmp = (uint64_t)n;
    seq_t psi = 1;

    /* Handle factor 2 separately */
    int count = 0;
    while ((tmp & 1ULL) == 0) { /* tmp % 2 == 0 */
        tmp >>= 1;
        count++;
    }
    if (count > 0) {
        /* part = 2^{count-1} * (2+1) */
        seq_t part = 1;
        for (int i = 0; i < count - 1; ++i) {
            part *= (seq_t)2;
        }
        part *= ((seq_t)2 + 1);
        psi *= part;
    }

    /* Factor odd primes */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p != 0) continue;
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        /* part = p^{e-1} * (p+1) */
        seq_t part = 1;
        for (int i = 0; i < e - 1; ++i) {
            part *= (seq_t)p;
        }
        part *= ((seq_t)p + 1);
        psi *= part;
    }

    /* tmp is now either 1 or a prime > sqrt(original n) */
    if (tmp > 1) {
        /* e = 1 for the remaining prime factor */
        seq_t part = ((seq_t)tmp + 1);
        psi *= part;
    }

    /* Compute gcd(1 + n, ψ(n)) */
    seq_t nseq = (seq_t)n;
    seq_t one_plus_n = nseq + 1;
    return gcd(one_plus_n, psi);
}
