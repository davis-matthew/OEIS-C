/*
 * A355525  Minimal difference between adjacent prime indices of n, or k if n
 *          is the k-th prime.
 * https://oeis.org/A355525
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 82 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal difference between adjacent prime indices of n, or k if n is the k‑th prime.
   For n >= 2: if n is prime, a(n) = index(n); otherwise
   a(n) = 0 if any prime factor has exponent > 1,
   else a(n) = minimal difference between consecutive prime indices
   of the distinct prime factors of n.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Prime case: return its prime index directly. */
    if (is_prime_big((seq_t)n))
        return prime_pi((idx_t)n);

    /* Factor n into distinct primes and exponents. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* If any exponent > 1, the repeated index gives 0 difference. */
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > 1)
            return (seq_t)0;

    /* Gather the indices of the distinct prime factors. */
    seq_t indices[64];
    size_t idxcnt = 0;
    for (size_t i = 0; i < cnt; ++i)
        indices[idxcnt++] = prime_pi((idx_t)pr[i]);

    /* Sort the indices (small array, simple bubble sort). */
    for (size_t i = 0; i < idxcnt; ++i)
        for (size_t j = i + 1; j < idxcnt; ++j)
            if (indices[i] > indices[j]) {
                seq_t tmp = indices[i];
                indices[i] = indices[j];
                indices[j] = tmp;
            }

    /* For a single index (should only occur for prime powers with exponent >1)
       the answer is 0, but that case is handled above. */
    if (idxcnt < 2)
        return (seq_t)0;

    /* Compute the minimal difference between consecutive indices. */
    seq_t min_diff = indices[1] - indices[0];
    for (size_t i = 2; i < idxcnt; ++i) {
        seq_t diff = indices[i] - indices[i - 1];
        if (diff < min_diff)
            min_diff = diff;
    }

    return min_diff;
}
