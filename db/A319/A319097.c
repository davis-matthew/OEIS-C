/*
 * A319097  One of the three successive approximations up to 7^n for 7-adic
 *          integer 6^(1/3). This is the 3 (mod 7) case (except for n = 0).
 * https://oeis.org/A319097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = unique k with 1 ≤ k ≤ 7ⁿ, k ≡ 3 (mod 7), and k³ ≡ 6 (mod
 *            7ⁿ)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* small helper for modular multiplication that works for both 128‑bit and true big ints */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
#ifdef __SIZEOF_INT128__
    return (seq_t)(((__uint128_t)a * b) % mod);
#else
    seq_t res = 0;
    while (b) {
        if (b & 1) res = (res + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return res;
#endif
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;

    seq_t k      = 3;        /* current solution modulo 7^(exp-1) */
    seq_t pow7   = 7;        /* equals 7^(exp-1) for the current exp */
    static const unsigned inv7[7] = {0,1,4,5,2,3,6};

    for (idx_t exp = 2; exp <= n; ++exp) {
        seq_t mod = pow7 * 7;                /* 7^exp */

        /* k³ (mod mod) */
        seq_t k2 = mul_mod(k, k, mod);
        seq_t k3 = mul_mod(k2, k, mod);

        /* f = k³ – 6 (mod mod) */
        seq_t f = (k3 + mod - 6) % mod;

        /* a = ((k³ – 6) / 7^(exp‑1)) mod 7 */
        seq_t a_val = (f / pow7) % 7;
        unsigned a_num = (unsigned)(a_val % 7);

        /* derivative 3*k² (mod 7) */
        unsigned k_mod7 = (unsigned)(k % 7);
        unsigned deriv  = (3 * (k_mod7 * k_mod7 % 7)) % 7;
        unsigned inv    = inv7[deriv];

        /* t = -(a * inv) mod 7 */
        unsigned t = (7 - (unsigned)((a_num * inv) % 7)) % 7;

        /* k = k + t * 7^(exp‑1) (mod 7^exp) */
        k = k + (seq_t)t * pow7;

        pow7 = mod;   /* prepare for next iteration */
    }
    return k;
}
