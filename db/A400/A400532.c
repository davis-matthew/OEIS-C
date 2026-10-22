/*
 * A400532  Square array read by downward antidiagonals: T(n,k) is the
 *          conductor of chi_{n,k}, where chi_{n,k} is the Dirichlet character
 *          modulo n with label k in the Conrey labeling system; T(n,k) = 0 if
 *          n and k are not coprime.
 * https://oeis.org/A400532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, new
 * Formula:   a(n) = T(i,j) where (i,j) is obtained by reading the square
 *            array * T(n,k) in downward antidiagonals (Cantor order). The
 *            index mapping is * as described in the OEIS entry for A400532
 * Range:     u8 n <= 9602, u16 n <= 9602, u32 n <= 9602, u64 n <= 9602,
 *            u128 n <= 9602, big any n
 * Helpers:   cantor_coords, gcd, mod_pow, phi, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- Standard headers ---- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- Utility headers (provided by the build system) ---- */
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/mod_pow.h"
#include "../utils/pow_uint.h"
#include "../utils/cantor_coords.h"

/* ---- Local helper: factorization of uint64_t numbers ---- */
static void factor_uint64(uint64_t n, uint64_t pr[], uint64_t ex[], size_t *cnt)
{
    size_t i = 0;
    if (n % 2ULL == 0ULL) {
        size_t e = 0;
        while (n % 2ULL == 0ULL) {
            n /= 2ULL;
            ++e;
        }
        pr[i] = 2ULL;
        ex[i] = e;
        ++i;
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            size_t e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            pr[i] = p;
            ex[i] = e;
            ++i;
        }
    }
    if (n > 1ULL) {
        pr[i] = n;
        ex[i] = 1;
        ++i;
    }
    *cnt = i;
}

/* ---- Compute multiplicative order of a modulo mod ---- */
static seq_t ord_seq_t(seq_t a, seq_t mod)
{
    if (mod == (seq_t)1)
        return 1;

    seq_t phi_mod = phi((idx_t)mod);          /* φ(mod) */
    uint64_t phi = (uint64_t)phi_mod;         /* phi fits in 64‑bit for our usage */

    /* Factor φ(mod) */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_uint64(phi, primes, exps, &pcnt);

    uint64_t d = phi;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t q = primes[i];
        /* Reduce d by prime factor q while a^(d/q) ≡ 1 (mod mod) */
        while (d % q == 0ULL) {
            seq_t t = mod_pow(a, (idx_t)(d / q), mod);
            if (t == (seq_t)1)
                d /= q;
            else
                break;
        }
    }
    return (seq_t)d;
}

/* ---- Conductor T(n,k) ---- */
static seq_t T_of_nk(uint64_t n, uint64_t k)
{
    if (gcd((seq_t)n, (seq_t)k) != (seq_t)1)
        return 0;

    /* Factorise n */
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_uint64(n, primes, exps, &pcnt);

    seq_t result = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t comp;

        if (p == 2ULL) {
            seq_t mod = pow_uint(2ULL, (idx_t)e);
            seq_t rem = (seq_t)k % mod;

            if (rem == (seq_t)1)
                comp = 1;
            else if (rem == mod - 1)
                comp = (seq_t)4;
            else {
                seq_t ordv = ord_seq_t(k, mod);
                comp = 4 * ordv;
            }
        } else {
            seq_t mod = pow_uint(p, (idx_t)e);
            seq_t rem = (seq_t)k % mod;

            if (rem == (seq_t)1)
                comp = 1;
            else {
                seq_t ordv = ord_seq_t(k, mod);
                seq_t pe_minus = pow_uint(p, (idx_t)(e - 1));
                seq_t g = gcd(ordv, pe_minus);
                comp = (seq_t)p * g;
            }
        }

        result *= comp;
    }
    return result;
}

/* ---- Main sequence function ---- */
seq_t a(idx_t n)
{
    uint64_t i, j;
    cantor_coords(n, &i, &j);
    return T_of_nk(i, j);
}
