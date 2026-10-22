/*
 * A340122  Odd composite integers m such that A001906(2*m-J(m,5)) == J(m,5)
 *          (mod m), where J(m,5) is the Jacobi symbol.
 * https://oeis.org/A340122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   lucas_U_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A340122: odd composite integers m such that
   U(2*m - J(m,5)) ≡ J(m,5) (mod m),
   where U is the Lucas sequence with parameters (a,b)=(3,1). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/lucas_U_mod.h"

/* ---- helper: 64‑bit modular multiplication and power ---- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((unsigned __int128)a * b) % mod);
}

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of range */
    static bool ready = false;
    static seq_t *buf = NULL;
    static size_t cap = 0, len = 0;
    static uint64_t last = 5;                /* last odd m tested */

    if (!ready) {
        ready = true;
        cap = 64;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    while (len < (size_t)n) {
        last += 2;                          /* next odd number */
        if (last % 5 == 0) continue;        /* Jacobi symbol would be 0 */
        if (is_prime_64(last)) continue;    /* skip primes */

        /* Jacobi symbol (last/5) for odd last coprime to 5 */
        uint64_t r = last % 5;
        int jacobi = (r == 1 || r == 4) ? 1 : -1;

        seq_t mod = (seq_t)last;
        seq_t k   = ((seq_t)2 * last) - (seq_t)jacobi;
        seq_t u   = lucas_U_mod(k, mod);
        seq_t target = (jacobi == 1) ? (seq_t)1 : (seq_t)(last - 1);

        if (u == target) {
            if (len == cap) {
                cap *= 2;
                buf = (seq_t*)realloc(buf, cap * sizeof(seq_t));
            }
            buf[len++] = (seq_t)last;
        }
    }
    return buf[n-1];
}
