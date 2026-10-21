/*
 * A259233  Random number table used by Doom.
 * https://oeis.org/A259233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, dumb, fini, full
 * Formula:   a(n) = floor((134775813^(n+1)-1)/8832667615232) mod 256
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --------- helper: 64‑bit modular multiplication ------------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)(((__uint128_t)a * b) % mod);
#else
    /* Fallback: use built‑in 128‑bit type if available */
    uint64_t res = 0;
    while (b) {
        if (b & 1) {
            if (res >= mod - a) res = res - (mod - a);
            else res += a;
        }
        if (a >= mod - a) a = (a << 1) - mod;
        else a <<= 1;
        b >>= 1;
    }
    return res;
#endif
}

/* --------- helper: 64‑bit modular exponentiation ------------ */
static inline uint64_t pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mul_mod(result, b, mod);
        b = mul_mod(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* --------- sequence function --------------------------------- */
seq_t a(idx_t n)
{
    /* constants */
    const uint64_t A = 134775813ULL;               /* multiplier  */
    const uint64_t BASE = A - 1;                  /* A - 1       */
    const uint64_t D   = BASE << 16;              /* (A-1)*2^16  */
    const uint64_t MOD = BASE << 24;              /* (A-1)*2^24  */

    /* compute A^(n+1) mod MOD */
    uint64_t p = pow_mod(A, (uint64_t)n + 1, MOD);

    /* x = A^(n+1) - 1  (mod MOD) */
    uint64_t y = (p + MOD - 1) % MOD;

    /* q = floor((A^(n+1)-1)/D)  ;  take it modulo 256 */
    uint64_t qmod256 = (y / D) & 0xFFULL;
    return (seq_t)qmod256;
}
