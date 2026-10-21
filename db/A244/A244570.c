/*
 * A244570  Smallest prime p >= prime(n) such that p*prime(n)+2 is prime.
 * https://oeis.org/A244570
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p >= prime(n) such that p*prime(n)+2 is
 *            prime
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 7061, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"      /* get_prime(idx_t n) -> uint32_t */
#include "../utils/next_prime.h"    /* next_prime(uint64_t n) -> uint64_t */
#include "../utils/isPrimeGeneric.h"/* isPrimeGeneric(seq_t n) -> bool */

/* Sequence entry for index n (offset 2).  For n < 2 the value is 0. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;          /* indices < 2 are unused */
    }

    /* The n-th prime.  get_prime returns a uint32_t, enough for n up to 10⁴. */
    uint32_t q = get_prime(n);

    /* Search for the smallest prime p ≥ q with p*q+2 prime. */
    uint64_t p = q;
    while (1) {
        /* Compute p*q+2 in the sequence type. */
        seq_t prod = (seq_t)p * (seq_t)q + (seq_t)2;

        if (isPrimeGeneric(prod)) {
            return (seq_t)p;
        }

        /* Move to the next prime candidate. */
        if (p == UINT64_MAX) {
            /* Unlikely for n ≤ 10⁴; fall back to brute search over odd numbers. */
            p++;
            while (!isPrimeGeneric((seq_t)p)) {
                p++;
            }
        } else {
            p = next_prime(p + 1);
        }
    }
}
