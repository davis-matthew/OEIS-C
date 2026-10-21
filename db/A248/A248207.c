/*
 * A248207  Primitive root of the multiplicative group mod n as returned by
 *          PARI version 2.7.2, or 0 when there is no primitive root.
 * https://oeis.org/A248207
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest primitive root of the multiplicative group
 *            modulo n as returned by PARI 2.7.2, or 0 when it does not exist.
 *            The function follows PARI’s algorithm for 2*p^k by correcting
 *            for even primitive roots
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"
#include "../utils/gcd.h"

/* factorise m into distinct prime factors returned in pr[],
   number of primes stored in *cnt. */
static void
factor_distinct(idx_t m, idx_t *pr, size_t *cnt)
{
    size_t c = 0;
    if (m % 2 == 0) {
        pr[c++] = 2;
        while (m % 2 == 0)
            m /= 2;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            pr[c++] = p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        pr[c++] = m;
    *cnt = c;
}

/* find the smallest primitive root modulo an odd prime power n = p^e. */
static seq_t
primroot_mod_prime_power(idx_t n, idx_t p, size_t e)
{
    /* φ(n) */
    idx_t phi = n;
    phi = phi / p * (p - 1);

    /* distinct prime factors of φ(n) */
    idx_t prime_factors[64];
    size_t pf_cnt = 0;
    factor_distinct(phi, prime_factors, &pf_cnt);

    /* search for smallest primitive root */
    for (idx_t g = 2; g < n; ++g) {
        if (gcd((seq_t)g, (seq_t)n) != 1)
            continue;
        if (mod_pow((seq_t)g, (uint64_t)phi, (seq_t)n) != 1)
            continue;
        int ok = 1;
        for (size_t i = 0; i < pf_cnt; ++i) {
            if (mod_pow((seq_t)g,
                        (uint64_t)(phi / prime_factors[i]),
                        (seq_t)n) == 1) {
                ok = 0;
                break;
            }
        }
        if (ok)
            return (seq_t)g;
    }
    return (seq_t)0;   /* should not happen for valid inputs */
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    if (n == 2)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)3;

    /* factorise n */
    idx_t pf[64];
    uint32_t pe[64];
    size_t pc = 0;
    idx_t tmp = n;

    if (tmp % 2 == 0) {
        pf[pc] = 2;
        pe[pc] = 0;
        while (tmp % 2 == 0) {
            pe[pc]++;
            tmp /= 2;
        }
        pc++;
    }

    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            pf[pc] = p;
            pe[pc] = 0;
            while (tmp % p == 0) {
                pe[pc]++;
                tmp /= p;
            }
            pc++;
        }
    }

    if (tmp > 1) {
        pf[pc] = tmp;
        pe[pc] = 1;
        pc++;
    }

    /* determine existence of primitive root */
    if (pc == 1) {               /* prime power */
        if (pf[0] == 2)
            return (seq_t)0;     /* 2^k with k>2 */
        return primroot_mod_prime_power(n, pf[0], pe[0]);
    }

    if (pc == 2) {               /* 2 * p^k */
        idx_t p = 0;
        size_t e = 0;
        if (pf[0] == 2) {
            if (pe[0] != 1)
                return (seq_t)0;
            p = pf[1];
            e = pe[1];
        } else if (pf[1] == 2) {
            if (pe[1] != 1)
                return (seq_t)0;
            p = pf[0];
            e = pe[0];
        } else
            return (seq_t)0;

        idx_t p_pow = 1;
        for (size_t i = 0; i < e; ++i)
            p_pow *= p;

        seq_t root = primroot_mod_prime_power(p_pow, p, e);

        idx_t root_raw = (idx_t)root;
        if ((root_raw & 1ULL) == 0)
            root_raw += p_pow;
        return (seq_t)root_raw;
    }

    return (seq_t)0;   /* no primitive root */
}
