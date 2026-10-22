/*
 * A341524  Number of prime factors in A017666(n), counted with multiplicity:
 *          a(n) = bigomega(n) - bigomega(gcd(n, sigma(n))).
 * https://oeis.org/A341524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(n) - bigomega(gcd(n, sigma(n)))
 * Range:     u8 n <= 107, u16 n <= 9622, u32 n <= 9622, u64 n <= 9622,
 *            u128 n <= 9622, big any n
 * Helpers:   bigomega, count_prime_factors, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);                 /* σ(n) */
    seq_t g = gcd((seq_t)n, sigma);            /* gcd(n, σ(n)) */
    seq_t omega_n = bigomega(n);               /* Ω(n) */
    seq_t omega_g = count_prime_factors(g);    /* Ω(g) */
    return omega_n - omega_g;                  /* difference is always ≥ 0 */
}
