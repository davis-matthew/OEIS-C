/*
 * A321903  Irregular table read by rows: T(n,k) = (2*k+1)^(-1/(2*k+1)) mod
 *          2^n, 0 <= k <= 2^(n-1) - 1.
 * https://oeis.org/A321903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_log2_uint64, inv_power_of_two, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A321903 – Irregular table read by rows
    T(n,k) = (2*k+1)^(-1/(2*k+1)) (mod 2^n)
    a(n) is the row‑major flattening of the table.
    The function below implements this definition using only integer
    arithmetic.  It works for arbitrary precision seq_t and for the
    usual 8–128‑bit unsigned integer builds.

    Algorithm:
      1. Determine the row r such that 2^(r-1) <= n < 2^r.
         r = floor_log2(n) + 1.
      2. Compute the column k = n - 2^(r-1).
      3. Set e = 2*k + 1  (the odd exponent).
      4. For r >= 3:
            - inv = e^(-1) (mod 2^r)  →  e_inv_mod
            - gamma = e^(-1) (mod 2^(r-2)) → e_inv_mod_phi
            - result = inv ^ gamma (mod 2^r)
         For r <= 2 we brute‑force the unique odd solution.
     The helper function inv_power_of_two performs Newton lifting to
     compute an inverse modulo a power of two.  All operations are
     performed modulo 2^r or 2^(r‑2). */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/mod_pow.h"
#include "../utils/inv_power_of_two.h"

/* Main function required by the judge */
seq_t a(idx_t n)
{
    /* ----------- Step 1: find row r and column k -------------- */
    idx_t r = floor_log2_uint64(n) + 1;      /* 1 <= r */
    idx_t k = n - ((idx_t)1 << (r - 1));     /* 0 <= k < 2^(r-1) */

    /* ----------- Step 2: compute the exponent e = 2*k+1 -------- */
    seq_t e = ((seq_t)2 * (seq_t)k) + (seq_t)1;   /* odd */

    /* ----------- Step 3: small r (1 or 2) --------------------- */
    if (r <= 2) {
        /* Brute force: find odd x < 2^r with x^e ≡ e^(-1) (mod 2^r) */
        seq_t mod = (seq_t)1 << r;
        seq_t inv_e = inv_power_of_two(e, r);  /* e^(-1) mod 2^r */
        for (seq_t x = (seq_t)1; x < mod; x += (seq_t)2) {
            if (mod_pow(x, (uint64_t)e, mod) == inv_e)
                return x;
        }
        /* Should never reach here */
        return (seq_t)0;
    }

    /* ----------- Step 4: general case r >= 3 ----------------- */

    /* Modulus 2^r */
    seq_t mod_r = (seq_t)1 << r;

    /* (a) Inverse of e modulo 2^r  */
    seq_t inv_e_mod = inv_power_of_two(e, r);

    /* (b) Inverse of e modulo 2^(r-2) used as exponent            */
    seq_t phi_mod = (seq_t)1 << (r - 2);           /* 2^(r-2)   */
    seq_t inv_e_mod_phi = inv_power_of_two(e, r - 2);

    /* (c) Final value: inv_e_mod ^ inv_e_mod_phi (mod 2^r) */
    return mod_pow(inv_e_mod, (uint64_t)inv_e_mod_phi, mod_r);
}
