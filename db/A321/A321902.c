/*
 * A321902  Irregular table read by rows: T(n,k) = (2*k+1)^(1/(2*k+1)) mod
 *          2^n, 0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A321902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row, k) where row = ⌊log2 n⌋ + 1 and k = n - 2^(row-1).
 *            T(row, k) is the unique odd integer x < 2^row such that x^(2k+1)
 *            ≡ 2k+1 (mod 2^row). For row ≥ 3 it is obtained by modular
 *            inversion: x = (2k+1)^(e⁻¹ mod 2^(row-2)) (mod 2^row), where e =
 *            2k+1 is odd. For row 1,2 we return 2k+1 dire...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   inv_mod_pow2, mod_pow_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/inv_mod_pow2.h"
#include "../utils/mod_pow_big.h"

/* Main sequence function for A321902. */
seq_t a(idx_t n)
{
    idx_t idx = n;      /* 1‑based index */
    if (idx == 0)
        return 0;       /* undefined, safe guard */

    /* Determine the row r and offset k for index idx. */
    unsigned r = 1;
    while (((idx_t)1 << r) - 1 < idx)
        ++r;                   /* r = floor(log2(idx)) + 1 */

    idx_t start = ((idx_t)1 << (r - 1));   /* first index in this row */
    idx_t k_value = idx - start;           /* 0 ≤ k_value < 2^(r-1) */

    seq_t k = (seq_t)k_value;              /* promote to seq_t for arithmetic */

    /* Handle the trivial first two rows directly. */
    if (r == 1 || r == 2)
        return ((seq_t)2 * k) + 1;         /* T(r,k) = 2k+1 for r≤2 */

    /* r >= 3: use modular inverse approach. */
    seq_t e   = ((seq_t)2 * k) + 1;                 /* exponent e = 2k+1 */
    seq_t mod = (seq_t)1 << r;                      /* modulus 2^r */
    unsigned lambda_bits = r - 2;                   /* λ = 2^(r-2) bits */

    /* Compute inverse of e modulo 2^(r-2). */
    seq_t e_inv = inv_mod_pow2(e, lambda_bits);    /* 0 ≤ e_inv < 2^(r-2) */

    /* Final result: x = a^e_inv (mod 2^r). */
    return mod_pow_big(e, e_inv, mod);
}
