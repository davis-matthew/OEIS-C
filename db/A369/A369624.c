/*
 * A369624  Numbers formed by the rightmost decimal digits of n^(n^n) that are
 *          the same as those of n^(n^(n^n)).
 * https://oeis.org/A369624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 4, i32 n <= 14, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A369624 – common right–most decimal digits of n^(n^n) and n^(n^(n^n)) */
#include <stdint.h>
#include <stdbool.h>

/* auxiliary power modulo for 64‑bit numbers (used for exponent reductions) */
static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* modular inverse of a modulo m (m is a power of 5 → co‑prime) */
static inline uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t = 0, newt = 1;
    int64_t r = m, newr = a % m;
    while (newr) {
        uint64_t q = r / newr;
        int64_t tmp = t - (int64_t)q * newt; t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr; r = newr; newr = tmp;
    }
    if (r > 1) return 0;            /* not invertible – should not occur */
    if (t < 0) t += m;
    return (uint64_t)t;
}

/* integer power 5^k (k ≤ 20, fits in 64 bit) */
static uint64_t pow5_u64(int k)
{
    uint64_t r = 1;
    for (int i = 0; i < k; ++i) r *= 5ULL;
    return r;
}

/* --------– modular parts of A(k) and B(k) -------------------- */

/* a2 = n^(n^n)  (mod 2^k)  */
static inline seq_t a_mod2(uint64_t n, int k)
{
    if ((n & 1ULL) == 0) return 0;
    uint64_t mod2 = 1ULL << k;
    uint64_t phi2 = 1ULL << (k-1);
    uint64_t exp1_mod = pow_mod_uint64(n, n, phi2);
    if (exp1_mod == 0) exp1_mod = phi2;
    return pow_mod_uint64(n % mod2, exp1_mod, mod2);
}

/* a5 = n^(n^n)  (mod 5^k)  */
static inline seq_t a_mod5(uint64_t n, int k)
{
    if (n % 5ULL == 0) return 0;
    uint64_t mod5k = pow5_u64(k);
    uint64_t phi5k = 4ULL * pow5_u64(k-1);
    uint64_t exp1_mod = pow_mod_uint64(n, n, phi5k);
    if (exp1_mod == 0) exp1_mod = phi5k;
    return pow_mod_uint64(n % mod5k, exp1_mod, mod5k);
}

/* b2 = n^(n^(n^n))  (mod 2^k)  */
static inline seq_t b_mod2(uint64_t n, int k)
{
    if ((n & 1ULL) == 0) return 0;
    if (k == 1) return 1;            /* n odd => 1 (mod 2) */
    uint64_t mod2  = 1ULL << k;
    uint64_t phi2  = 1ULL << (k-1);
    uint64_t phi_phi2 = (k <= 2) ? 1ULL : (1ULL << (k-2));

    uint64_t exp1_mod_phi_phi2 =
        (phi_phi2 == 1) ? 0 : pow_mod_uint64(n, n, phi_phi2);
    if (exp1_mod_phi_phi2 == 0 && phi_phi2 > 1) exp1_mod_phi_phi2 = phi_phi2;

    uint64_t exp2_mod = pow_mod_uint64(n % phi2, exp1_mod_phi_phi2, phi2);
    if (exp2_mod == 0) exp2_mod = phi2;   /* exponent ≡0  →  use φ(2^k) */

    return pow_mod_uint64(n % mod2, exp2_mod, mod2);
}

/* b5 = n^(n^(n^n))  (mod 5^k)  */
static inline seq_t b_mod5(uint64_t n, int k)
{
    if (n % 5ULL == 0) return 0;
    uint64_t mod5k = pow5_u64(k);
    uint64_t phi5k = 4ULL * pow5_u64(k-1);

    uint64_t phi_phi5k;
    if (k == 1)      phi_phi5k = 2ULL;
    else if (k == 2) phi_phi5k = 8ULL;
    else             phi_phi5k = 8ULL * pow5_u64(k-2);

    uint64_t exp1_mod_phi_phi5k =
        (phi_phi5k == 1) ? 0 : pow_mod_uint64(n, n, phi_phi5k);
    if (exp1_mod_phi_phi5k == 0 && phi_phi5k > 1)
        exp1_mod_phi_phi5k = phi_phi5k;

    uint64_t exp2_mod = pow_mod_uint64(n % phi5k, exp1_mod_phi_phi5k, phi5k);
    if (exp2_mod == 0) exp2_mod = phi5k;

    return pow_mod_uint64(n % mod5k, exp2_mod, mod5k);
}

/* Chinese Remainder: recover x such that
   x ≡ a(mod m1) and x ≡ b(mod m2)  (m1,m2 coprime) */
static inline seq_t crt(seq_t a, seq_t b, seq_t m1, seq_t m2)
{
    uint64_t a1 = (uint64_t)a;
    uint64_t a2 = (uint64_t)b;
    uint64_t m5 = (uint64_t)m2;

    uint64_t t0 = (a2 + m5 - a1) % m5;
    uint64_t inv = modinv_uint64((uint64_t)(m1 % m5), m5);
    uint64_t t  = (__int128)t0 * inv % m5;
    return a + (seq_t)t * m1;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t N = (uint64_t)n;
    seq_t prev = 0;
    seq_t mod10 = 1;

    for (int k = 1; k <= 18; ++k) {        /* 10^18 < 2^63 */
        mod10 *= 10;                       /* 10^k in seq_t */

        seq_t mod2 = (seq_t)((uint64_t)1 << k);
        seq_t mod5 = (seq_t)pow5_u64(k);

        seq_t a2 = a_mod2(N, k);
        seq_t a5 = a_mod5(N, k);
        seq_t b2 = b_mod2(N, k);
        seq_t b5 = b_mod5(N, k);

        seq_t a_val = crt(a2, a5, mod2, mod5);
        seq_t b_val = crt(b2, b5, mod2, mod5);

        if (a_val != b_val) return prev;
        prev = a_val;
    }
    return prev;   /* should never reach here for the tested range */
}
