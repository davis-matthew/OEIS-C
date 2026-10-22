/*
 * A318440  a(n) = A046645(n) - A007814(n); the 2-adic valuation of A299150.
 * https://oeis.org/A318440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9742, u16 n <= 9742, u32 n <= 9742, u64 n <= 9742,
 *            u128 n <= 9742, big any n
 * Helpers:   gcd_u64, mulmod_u64, pollard_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318440
 *
 *  a(n) = A046645(n) - A007814(n)
 *       = sum over prime powers dividing n of
 *         ((1 + (p mod 2)) * e - A000120(e))
 *       where p^e || n and A000120(e) = popcount(e).
 *  The function below computes a(n) for any n ≥ 1 using 64‑bit
 *  arithmetic and Pollard Rho factorisation.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"
#include "../utils/mulmod_u64.h"
#include "../utils/pollard_u64.h"

/* --------------------------------------------------------------- */
/*  Utility arithmetic on 64‑bit unsigned integers (C/C++ hybrid).  */
/* --------------------------------------------------------------- */

/* Modular multiplication: (a*b) % mod using 128‑bit intermediate.   */

/* Modular exponentiation: a^e mod mod.                              */
static uint64_t powmod_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mulmod_u64(r, a, mod);
        a = mulmod_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Greatest common divisor – Euclidean algorithm.                    */

/* --------------------------------------------------------------- */
/*  Miller–Rabin primality test for 64‑bit unsigned integers.       */
/*  Deterministic with the 7 bases known to be enough.              */
/* --------------------------------------------------------------- */
static int is_prime_u64(uint64_t n)
{
    if (n < 2) return 0;
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    for (int i = 0; i < 12; ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return 1;
        if (n % p == 0) return 0;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1; ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (int i = 0; i < 7; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod_u64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int comp = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
            if (x == n - 1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* --------------------------------------------------------------- */
/*  Pollard Rho factorisation.                                      */

/* --------------------------------------------------------------- */
/*  Recursive prime factorisation into an array (multiplicity 1).  */
/* --------------------------------------------------------------- */
static void factor_u64(uint64_t n, uint64_t *pf, int *cnt)
{
    if (n == 1) return;
    if (is_prime_u64(n)) {
        pf[(*cnt)++] = n;
        return;
    }
    uint64_t d = pollard_u64(n);
    factor_u64(d, pf, cnt);
    factor_u64(n / d, pf, cnt);
}

/* --------------------------------------------------------------- */
/*  Main sequence function.                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(1) = 0 */

    uint64_t pf[64];   /* prime factors (with multiplicity 1)   */
    int pf_cnt = 0;
    factor_u64((uint64_t)n, pf, &pf_cnt);

    /* Sort the factors to group equal primes. */
    for (int i = 1; i < pf_cnt; ++i) {
        uint64_t key = pf[i];
        int j = i - 1;
        while (j >= 0 && pf[j] > key) {
            pf[j + 1] = pf[j];
            --j;
        }
        pf[j + 1] = key;
    }

    seq_t res = 0;
    int i = 0;
    while (i < pf_cnt) {
        uint64_t p = pf[i];
        int e = 0;
        while (i < pf_cnt && pf[i] == p) {
            ++e; ++i;
        }
        seq_t term = ((seq_t)(1 + (p & 1ULL)) * (seq_t)e)
                     - (seq_t)__builtin_popcountll((unsigned long long)e);
        res += term;
    }
    return res;
}
