/*
 * A324084  One of the four successive approximations up to 13^n for the
 *          13-adic integer 3^(1/4). This is the 11 (mod 13) case (except for
 *          n = 0).
 * https://oeis.org/A324084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique integer k ≡ 11 (mod 13) with k⁴ ≡ 3 (mod
 *            13ⁿ). * Computation is by successive Hensel lifting: given a
 *            solution mod 13ᵐ, * obtain the solution mod 13ᵐ⁺¹
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small_int_seq, mul_small_int_small, pow_mul_seq,
 *            pow_mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small_int_seq.h"
#include "../utils/mul_small_int_small.h"
#include "../utils/pow_mul_seq.h"
#include "../utils/pow_mul_small.h"

/* helper: small‑integer modular multiplication ---------------------------------- */
static inline seq_t mul_mod_small(uint64_t a, uint64_t b, uint64_t mod) {
    unsigned __int128 prod = (unsigned __int128)a * b;
    return (seq_t)(prod % mod);
}

/* helper: arbitrary‑precision modular multiplication --------------------------------- */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod) {
    return (a * b) % mod;
}

/* main sequence function ------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 11;

    seq_t x   = 11;   /* current solution */
    seq_t pow = 13;   /* 13^m, where m is the current power index (starting at 1) */
    const int inv_fprime = 2; /* inverse of 7 modulo 13 (since 7·2≡1 mod13) */

    for (idx_t m = 1; m < n; ++m) {
        /* modulus for this lifting step: 13^(m+1) = pow * 13 */
        seq_t mod;
        if (sizeof(seq_t) <= 8) {
            mod = pow_mul_small((uint64_t)pow);  /* cast safe for small types */
        } else {
            mod = pow_mul_seq(pow);
        }

        /* compute x² mod mod */
        seq_t x2;
        if (sizeof(seq_t) <= 8) {
            x2 = mul_mod_small((uint64_t)x, (uint64_t)x, (uint64_t)mod);
        } else {
            x2 = mul_mod_seq(x, x, mod);
        }

        /* compute x⁴ mod mod */
        seq_t x4;
        if (sizeof(seq_t) <= 8) {
            x4 = mul_mod_small((uint64_t)x2, (uint64_t)x2, (uint64_t)mod);
        } else {
            x4 = mul_mod_seq(x2, x2, mod);
        }

        /* f = (x⁴ - 3) mod mod, guaranteed non‑negative */
        seq_t f;
        if (x4 >= 3) {
            f = x4 - 3;
        } else {
            if (sizeof(seq_t) <= 8) {
                f = (seq_t)((uint64_t)mod + (uint64_t)x4 - 3);
            } else {
                f = mod + x4 - 3;
            }
        }

        /* d_mod13 = (f / pow) mod 13 */
        seq_t div = f / pow;                /* exact division by 13ᵐ */
        int d_mod13 = (int)(div % 13);       /* remainder modulo 13 */

        /* solve for t in 13t ≡ -(d_mod13 * inv_fprime) (mod 13) */
        int t = (-d_mod13 * inv_fprime) % 13;
        if (t < 0) t += 13;

        /* compute delta = t * pow (exact) */
        seq_t delta;
        if (sizeof(seq_t) <= 8) {
            delta = mul_small_int_small((uint64_t)pow, (unsigned int)t);
        } else {
            delta = mul_small_int_seq(pow, (unsigned int)t);
        }

        /* update x and pow for next step */
        if (sizeof(seq_t) <= 8) {
            x   = (seq_t)((unsigned __int128)x + delta);
            pow = pow_mul_small((uint64_t)pow);
        } else {
            x   = x + delta;
            pow = pow_mul_seq(pow);
        }
    }
    return x;
}
