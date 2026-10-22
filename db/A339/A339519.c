/*
 * A339519  Odd composite integers m such that A087130(2*m-J(m,29)) ==
 *          5*J(m,29) (mod m), where J(m,29) is the Jacobi symbol.
 * https://oeis.org/A339519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339519: odd composite integers m such that
   A087130(2*m - J(m,29)) ≡ 5*J(m,29) (mod m).
   J(m,29) is the Jacobi symbol (m|29).
   This implementation searches for suitable values on demand
   and memoises the results.  All arithmetic is performed modulo m
   using 128‑bit intermediate products to avoid overflow on 64‑bit
   machines.  The function returns cache[n-1] for n≥1. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/append_candidate.h"

/* 2×2 matrix for the recurrence V(n+2)=5·V(n+1)+V(n)  */
typedef struct { uint64_t a00, a01, a10, a11; } mat2;

/* matrix multiplication modulo 'mod' (mod < 2^64) */
static mat2 mat_mul(mat2 A, mat2 B, uint64_t mod)
{
    mat2 C;
    C.a00 = (uint64_t)(((__uint128_t)A.a00 * B.a00 +
                       (__uint128_t)A.a01 * B.a10) % mod);
    C.a01 = (uint64_t)(((__uint128_t)A.a00 * B.a01 +
                       (__uint128_t)A.a01 * B.a11) % mod);
    C.a10 = (uint64_t)(((__uint128_t)A.a10 * B.a00 +
                       (__uint128_t)A.a11 * B.a10) % mod);
    C.a11 = (uint64_t)(((__uint128_t)A.a10 * B.a01 +
                       (__uint128_t)A.a11 * B.a11) % mod);
    return C;
}

/* fast exponentiation of a 2×2 matrix modulo 'mod' */
static mat2 mat_pow(mat2 M, idx_t exp, uint64_t mod)
{
    mat2 result = {1 % mod, 0, 0, 1 % mod};
    while (exp) {
        if (exp & 1U)
            result = mat_mul(result, M, mod);
        M = mat_mul(M, M, mod);
        exp >>= 1U;
    }
    return result;
}

/* compute V(k) mod 'mod' for k ≥ 0:
     V(0)=2, V(1)=5, V(n+2)=5·V(n+1)+V(n)              */
static seq_t V_mod_u64(idx_t k, uint64_t mod)
{
    if (mod == 1) return (seq_t)0;
    if (k == 0) return (seq_t)(2 % mod);
    if (k == 1) return (seq_t)(5 % mod);

    mat2 M = {5 % mod, 1 % mod, 1 % mod, 0};
    mat2 P = mat_pow(M, k - 1, mod);
    uint64_t five = 5 % mod;
    uint64_t two  = 2 % mod;
    uint64_t res = (uint64_t)(((__uint128_t)P.a00 * five +
                               (__uint128_t)P.a01 * two) % mod);
    return (seq_t)res;
}

/* sequence a(n) ------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache   = NULL;          /* cached terms          */
    static size_t cache_len = 0;           /* number of terms cached*/
    static size_t cache_cap = 0;           /* allocated capacity     */
    static uint64_t cand = 9;              /* next odd integer to test */

    while (cache_len < n) {
        /* ---------------------------------------------------------- */
        /*  Find the next odd composite m satisfying the condition.  */
        /* ---------------------------------------------------------- */
        for (;;) {
            uint64_t m = cand;
            cand += 2;                     /* next odd number */

            /* skip primes */
            if (is_prime_big(m))
                continue;

            /* Jacobi symbol J(m,29) */
            int J = jacobi_symbol(m, 29);

            /* k = 2*m - J(m,29) */
            idx_t k = (idx_t)((uint64_t)m << 1);
            if (J ==  1) k -= 1;
            else if (J == -1) k += 1;

            /* V(k) mod m */
            seq_t val = V_mod_u64(k, m);

            /* target = 5*J(m,29) (mod m) */
            seq_t target;
            if (J ==  1) target = (seq_t)5 % m;
            else if (J == -1)
                target = ((seq_t)m - ((seq_t)5 % m)) % m;
            else                       /* J == 0 */
                target = 0;

            /* check if val ≡ target (mod m) */
            uint64_t val64  = (uint64_t)val;      /* val < m */
            uint64_t tgt64  = (uint64_t)target;  /* target < m */
            if ( ( ( (__uint128_t)val64 + m - tgt64 ) % m ) == 0 ) {
                append_candidate(&cache, &cache_len, &cache_cap, (seq_t)m);
                break;                          /* next term found  */
            }
        }
    }
    return cache[n - 1];                      /* a(1) is cache[0] */
}
