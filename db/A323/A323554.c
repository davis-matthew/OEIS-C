/*
 * A323554  Irregular table read by rows: T(n,k) = (2*k+1)^(-1/5) mod 2^n, 0
 *          <= k <= 2^(n-1) - 1.
 * https://oeis.org/A323554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 9950, u32 n <= 9950, u64 n <= 9950,
 *            u128 n <= 9950, big any n
 * Helpers:   mask_uint, mulmod2n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A323554: a(n) is the value read from the irregular table
   of odd residues x modulo 2^r satisfying x^5 · (2k+1) ≡ 1 (mod 2^r),
   where r = floor(log2(n)) + 1 and k = n – 2^(r‑1). */
#include <stdint.h>
#include "../utils/mask_uint.h"
#include "../utils/mulmod2n.h"

/* Inverse of odd a modulo 2^bits using Newton lifting. */
static inline seq_t inv_mod_pow2(seq_t a, int bits)
{
    seq_t inv = 1;
    seq_t mask = mask_uint(bits);
    for (int i = 0; i < bits; ++i) {
        seq_t prod = mulmod2n(a, inv, bits);          /* a * inv  (mod 2^bits) */
        seq_t sec  = ( (seq_t)2 - prod ) & mask;      /* 2 – a*inv (mod 2^bits) */
        inv = mulmod2n(inv, sec, bits);              /* inv = inv * sec (mod 2^bits) */
    }
    return inv;
}

/* Modulo 2^bits exponentiation using square‑and‑multiply. */
static inline seq_t modpow2(seq_t base, uint64_t exp, int bits)
{
    seq_t mask = mask_uint(bits);
    base &= mask;
    seq_t result = 1;
    while (exp) {
        if (exp & 1U) result = mulmod2n(result, base, bits);
        base = mulmod2n(base, base, bits);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* value below offset is irrelevant */

    /* Determine row r and column k. */
    idx_t tmp = n;
    int r = 0;
    while (tmp > 0) { r++; tmp >>= 1; }          /* r = floor(log2 n) + 1 */
    idx_t start = ((idx_t)1 << (r - 1));
    idx_t k = n - start;

    /* Compute 2k+1 as odd base. */
    seq_t base = ((seq_t)2 * (seq_t)k + (seq_t)1);

    if (r == 1) return (seq_t)1;

    /* Inverse of (2k+1) modulo 2^r. */
    seq_t inv_a = inv_mod_pow2(base, r);

    if (r == 2) return inv_a;                  /* for r=2 the 5th‑root is trivial */

    /* Compute inverse of 5 modulo 2^(r-2). */
    seq_t inv5 = inv_mod_pow2((seq_t)5, r - 2);
    uint64_t exp = (uint64_t)inv5;              /* exponent fits into 64 bits for r≤64 */

    /* 5th‑root of inv_a modulo 2^r: inv_a^inv5 mod 2^r. */
    return modpow2(inv_a, exp, r);
}
