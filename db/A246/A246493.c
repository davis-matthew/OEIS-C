/*
 * A246493  Smallest k such that 5^^n is not congruent to 5^^(n-1) mod k,
 *          where 5^^n denotes the power tower 5^5^...^5 (in which 5 appears n
 *          times).
 * https://oeis.org/A246493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the smallest integer k ≥ 2 such that 5^^n ≠ 5^^(n-1) (mod
 *            k), where 5^^n is the power tower 5^5^…^5 (n times). For n ≥ 9
 *            the sequence follows a(n) = 3^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   phi_uint32, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* external utilities that may be available */
#include "../utils/seq_pow.h"      /* seq_pow(seq_t base, idx_t exp) */
#include "../utils/phi_uint32.h"

/* ---------- helper functions ---------- */

/* binary modular exponentiation (base, exp, mod) for uint64_t values. */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* 5^^n mod mod, for n ≥ 1.  The function works for all n up to 8
   (the range needed for the search part).  It uses the standard
   form of exponent reduction with Euler’s theorem. */
static uint64_t mod_tetration_uint64(idx_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    if (n == 1) return 5 % mod;
    if (n == 2) return powmod_uint64(5, 5, mod);

    uint32_t phi = phi_uint32((uint32_t)mod);
    uint64_t exp = mod_tetration_uint64(n - 1, phi);
    /* actual exponent is > phi, so add phi to preserve congruence */
    uint64_t exponent = exp + phi;
    return powmod_uint64(5, exponent, mod);
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    /* For n ≥ 9 the sequence equals 3^n. */
    if (n >= 9) return seq_pow((seq_t)3, n);

    /* Search for the smallest k that distinguishes the two tetrations. */
    for (uint64_t k = 2; ; ++k) {
        uint64_t prev, curr;
        if (n == 1) {
            prev = 1 % k;          /* 5^^0 = 1 */
            curr = 5 % k;
        } else {
            prev = mod_tetration_uint64(n - 1, k);
            curr = mod_tetration_uint64(n,     k);
        }
        if (curr != prev) return (seq_t)k;
    }
}
