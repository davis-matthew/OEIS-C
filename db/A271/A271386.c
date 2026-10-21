/*
 * A271386  Discriminants of the polynomials T_n(x) = Product_{k=0..n} (x -
 *          k*(k + 1)/2).
 * https://oeis.org/A271386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A271386: discriminants of T_n(x) = ∏(x - k(k+1)/2)
   Formula: D_n = ∏_{0≤i<j≤n} (T_j - T_i)^2
           = 2^{-(n+1)n} · ∏_{d=1}^n d^{2(n+1-d)} · ∏_{s=2}^{2n} s^{2m_s},
   where m_s = #{(i,j): i<j, i+j+1=s}.  The code computes this
   product exactly using only integer operations. */

#include <stdint.h>                     /* for uint64_t */
#include "../utils/seq_pow.h"           /* seq_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"           /* seq_mul(seq_t, seq_t) */

/* Count trailing zeros of a 64‑bit unsigned integer. */
static inline unsigned int v2_uint64(uint64_t x) {
    unsigned int v = 0;
    while ((x & 1ULL) == 0ULL) {
        x >>= 1;
        ++v;
    }
    return v;
}

seq_t a(idx_t n) {
    seq_t result = 1;                   /* multiplicative identity */

    unsigned long long exp2_num = 0;    /* exponent of 2 appearing in numerator */

    /* ---- part 1 : product over differences (j - i) ---- */
    for (idx_t d = 1; d <= n; ++d) {
        idx_t exp = 2ULL * (n + 1 - d);  /* exponent of d in the product */
        if (exp == 0) continue;

        uint64_t u = d;
        unsigned int v2 = v2_uint64(u);
        uint64_t odd = u >> v2;

        exp2_num += (unsigned long long)v2 * (unsigned long long)exp;

        if (odd != 1ULL) {              /* odd part ^ exp */
            seq_t power = seq_pow((seq_t)odd, exp);
            result = seq_mul(result, power);
        }
    }

    /* ---- part 2 : product over sums (j + i + 1) ---- */
    for (idx_t s = 2; s <= 2 * n; ++s) {
        idx_t sum = s - 1;                           /* i + j = sum */
        idx_t low = (sum > n) ? (sum - n) : 0;       /* minimal i allowed */
        idx_t high = (sum - 1) / 2;                  /* maximal i allowed */

        if (high < low) continue;                    /* no pairs with this s */

        idx_t m = high - low + 1;                    /* multiplicity of s */
        idx_t exp = 2ULL * m;                        /* exponent of s */

        uint64_t u = s;
        unsigned int v2 = v2_uint64(u);
        uint64_t odd = u >> v2;

        exp2_num += (unsigned long long)v2 * (unsigned long long)exp;

        if (odd != 1ULL) {                            /* odd part ^ exp */
            seq_t power = seq_pow((seq_t)odd, exp);
            result = seq_mul(result, power);
        }
    }

    /* ---- final factor of 2 ---- */
    long long exp2_final = (long long)exp2_num
                           - (long long)n * (long long)(n + 1);
    seq_t pow2 = seq_pow((seq_t)2, (idx_t)exp2_final);
    result = seq_mul(result, pow2);

    return result;
}
