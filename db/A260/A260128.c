/*
 * A260128  Primes having only {2, 3, 9} as digits.
 * https://oeis.org/A260128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 56, u32 n <= 5672, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that consists only of the decimal digits 2, 3, and 9. */
/* The sequence starts 2, 3, 23, 29, 223, 229, 233, 239, ...          */
/* This implementation uses a deterministic Miller–Rabin test for 64‑bit
   numbers, generating candidates by increasing length until the n‑th
   prime is found. */

#include <stdint.h>
#include <stdbool.h>

/* ----------  Miller–Rabin for 64‑bit positive integers ---------- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes that we test explicitly */
    const uint64_t small_primes[7] = {2, 3, 5, 7, 11, 13, 17};
    for (int i = 0; i < 7; ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    /* Write n‑1 as d·2^s  with d odd. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Deterministic witnesses for 64‑bit integers */
    for (int i = 0; i < 7; ++i) {
        uint64_t a = small_primes[i];
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ----------  Recursive candidate generator ---------- */
static bool gen_rec(idx_t len, idx_t pos, uint64_t cur,
                    idx_t target, idx_t *cnt, seq_t *result)
{
    if (pos == len) {                         /* full candidate built   */
        if (is_prime_u64(cur)) {               /* check primality        */
            ++(*cnt);
            if (*cnt == target) {              /* n‑th prime found      */
                *result = (seq_t)cur;
                return true;
            }
        }
        return false;
    }
    static const int digits[3] = {2, 3, 9};
    for (int d_idx = 0; d_idx < 3; ++d_idx) {
        int d = digits[d_idx];
        if (len > 1 && pos == len - 1 && d == 2) continue; /* last digit even */
        if (gen_rec(len, pos + 1, cur * 10 + d,
                    target, cnt, result))
            return true;
    }
    return false;
}

/* ----------  Main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  result = 0;
    for (idx_t len = 1; ; ++len) {               /* increase digit length   */
        if (gen_rec(len, 0, 0ULL, n, &count, &result))
            return result;
    }
}
