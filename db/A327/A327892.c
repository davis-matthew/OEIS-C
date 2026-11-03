/*
 * A327892  a(n) is the smallest positive integer k such that for every prime
 *          divisor p_i of 2*n+1, p_i + k is also prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A327892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k>0 such that for every prime divisor p of 2*n+1,
 *            p+k is prime; a(n)=0 if no such k exists (e.g. residue condition
 *            holds)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
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

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n) {
    /* m = 2*n+1 */
    uint64_t m = 2ull * (uint64_t)n + 1ull;

    /* factor m */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    /* if m is prime */
    if (cnt == 1 && ex[0] == 1) {
        uint64_t k = 1;
        uint64_t candidate = m + k;
        while (!is_prime_big(candidate))
            ++candidate;
        return (seq_t)(candidate - m);
    }

    /* smallest prime factor */
    uint64_t p_min = pr[0];
    /* If p_min appears in multiple prime factors? primes are unique from factor_u64 */

    /* Check residue condition: if all residues mod p_min are present, return 0 */
    size_t bits = (size_t)p_min;
    size_t words = (bits + 63) >> 6;
    uint64_t *mask = (uint64_t*)calloc(words, sizeof(uint64_t));
    if (!mask) return 0; /* fallback on allocation failure */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t r = pr[i] % p_min;
        size_t idx = r >> 6;
        size_t bit = r & 63;
        mask[idx] |= (1ull << bit);
    }
    /* check if all bits set for residues 0..p_min-1 */
    bool all_res = true;
    for (size_t i = 0; i < words; ++i) {
        uint64_t expected = ~0ull;
        if (i == words - 1 && bits & 63) {
            expected = (1ull << (bits & 63)) - 1;
        }
        if ((mask[i] & expected) != expected) {
            all_res = false;
            break;
        }
    }
    free(mask);
    if (all_res) return 0;

    /* Search for minimal k */
    const uint64_t K_LIMIT = 1000000ull; /* safe upper bound for these sizes */
    for (uint64_t k = 1; k <= K_LIMIT; ++k) {
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t test = (seq_t)pr[i] + (seq_t)k;
            if (!is_prime_big(test)) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;
    }

    /* If no k found within limit, return 0 (unlikely for given bounds) */
    return 0;
}
