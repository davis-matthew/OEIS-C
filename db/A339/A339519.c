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
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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
