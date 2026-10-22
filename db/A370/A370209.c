/*
 * A370209  a(n) is the smallest number of the form 2^k * p * (2^(k+1) * p +
 *          1) where 2 < p < 2^(k+1) is the n-th prime and 2^(k+1) * p + 1 is
 *          prime, or -1 if no such number exists.
 * https://oeis.org/A370209
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   floor_log2_uint64, get_prime, is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min{ 2^k * p * ( 2^(k+1) * p + 1 )
 *                      : p = prime(n), k ≥ floor(log2(p)),
 *                        2^(k+1) * p + 1 is prime }, n ≥ 2
 */

#include <stdbool.h>
#include "../utils/get_prime.h"          /* uint32_t get_prime(idx_t k); */
#include "../utils/floor_log2_uint64.h"  /* unsigned floor_log2_uint64(idx_t v); */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n); */
#include "../utils/pow2.h"               /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)-1;                /* undefined for n < 2 */
    }

    /* --------- obtain the n‑th prime (p > 2) ----------------- */
    uint32_t p_u = get_prime(n);          /* p fits in a 32‑bit value */
    idx_t p = p_u;                         /* convert for floor_log2 */

    /* --------- start at the smallest k such that 2^(k+1) > p ------ */
    unsigned k_start = floor_log2_uint64(p); /* floor(log2(p)) */
    idx_t k = k_start;

    /* --------- compute initial powers and q -------------------- */
    seq_t pow2k  = pow2(k);               /* 2^k */
    seq_t pow2k1 = pow2k << 1;            /* 2^(k+1) */
    seq_t q      = pow2k1 * (seq_t)p + 1; /* 2^(k+1)*p + 1 */

    /* --------- increase k until q is prime --------------------- */
    while (!is_prime_big(q)) {
        k++;
        pow2k  = pow2k  << 1;             /* double 2^k  */
        pow2k1 = pow2k1 << 1;             /* double 2^(k+1) */
        q = pow2k1 * (seq_t)p + 1;        /* recompute q   */
    }

    /* --------- return the minimal 2^k * p * q ------------------ */
    return pow2k * (seq_t)p * q;
}
