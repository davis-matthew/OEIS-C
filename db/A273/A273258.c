/*
 * A273258  Write the distinct prime divisors p of n in the (PrimePi(p) -
 *          1)-th place, ignoring multiplicity. Decode the resulting number
 *          after first reversing the code, ignoring any leading zeros.
 * https://oeis.org/A273258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = A019565(A030101(A087207(n)))
 * Range:     u8 n <= 77, u16 n <= 1109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: encode n by writing "1" in the (PrimePi(p)-1)-th place for each distinct p|n,
   reverse the binary string, drop leading zeros, and decode as the product of primes
   whose indices correspond to the remaining "1" positions. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_pi.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* f(1) = "0" -> result 1 */

    /* factor n into distinct primes */
    idx_t temp = n;
    const idx_t MAX_FACTORS = 32;
    idx_t idx[MAX_FACTORS];
    idx_t cnt = 0;
    idx_t max_index = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* distinct prime p found */
            while (temp % p == 0) temp /= p;
            idx_t pi_p = (idx_t)compute_pi(p);   /* pi(p) */
            idx_t i = pi_p - 1;                  /* 0‑based index */
            idx[cnt++] = i;
            if (i > max_index) max_index = i;
        }
    }
    if (temp > 1) {            /* last prime factor > sqrt */
        idx_t pi_p = (idx_t)compute_pi(temp);
        idx_t i = pi_p - 1;
        idx[cnt++] = i;
        if (i > max_index) max_index = i;
    }

    /* decode by reversing indices and multiplying the corresponding primes */
    seq_t result = 1;
    for (idx_t k = 0; k < cnt; ++k) {
        idx_t i = idx[k];
        idx_t j = max_index - i;                  /* reversed position */
        seq_t prime_j = (seq_t)get_prime((idx_t)j + 1);  /* j+1-th prime, 1‑based */
        result = (seq_t)((unsigned __int128)result *
                          (unsigned __int128)prime_j);
    }
    return result;
}
