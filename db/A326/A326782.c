/*
 * A326782  Numbers whose binary indices are prime numbers.
 * https://oeis.org/A326782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i : bit i of (n‑1) is set} 2^{p_{i+1}−1} where p_k is
 *            the k‑th prime. The sequence starts with a(1)=0, a(2)=2, a(3)=4,
 *            a(4)=6, a(5)=16, …
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 2048, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

/* first 200 primes – plenty for n≤100 000 (needs < 200 bits). */
static const unsigned int primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
    53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
    101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
    151, 157, 163, 167, 173, 179, 181, 191, 193, 197,
    199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
    263, 269, 271, 277, 281, 283, 293, 307, 311, 313,
    317, 331, 337, 347, 349, 353, 359, 367, 373, 379,
    383, 389, 397, 401, 409, 419, 421, 431, 433, 439,
    443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
    503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
    577, 587, 593, 599, 601, 607, 613, 617, 619, 631,
    641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
    701, 709, 719, 727, 733, 739, 743, 751, 757, 761,
    769, 773, 787, 797, 809, 811, 821, 823, 827, 829,
    839, 853, 857, 859, 863, 877, 881, 883, 887, 907,
    911, 919, 929, 937, 941, 947, 953, 967, 971, 977,
    983, 991, 997
};
static const size_t primes_count = sizeof(primes) / sizeof(primes[0]);

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 0 */
        return (seq_t)0;
    }

    idx_t k = n - 1;            /* bits of (n-1) */
    seq_t result = 0;
    idx_t bit = 0;              /* bit position in k */

    while (k) {
        if (k & (idx_t)1) {
            /* add 2^{prime(bit+1)-1} */
            result += pow2((idx_t)(primes[bit] - 1));
        }
        ++bit;
        k >>= 1;

        /* safety: if we run out of primes, stop */
        if (bit >= (idx_t)primes_count) {
            break;
        }
    }

    return result;
}
