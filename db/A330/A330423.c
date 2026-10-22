/*
 * A330423  Least nonsquare k that is a quadratic residue modulo n and is
 *          coprime to n.
 * https://oeis.org/A330423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the least k that is a quadratic residue modulo n,
 * coprime to n and not a perfect square.  The value is found by
 * incrementally testing k starting at 2, using prime factorisation of n
 * and Legendre symbols for odd primes.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* ---------- small helpers ------------------------------------------ */

static uint64_t mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    unsigned __int128 t = (unsigned __int128)a * b;
    return (uint64_t)(t % mod);
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e)
    {
        if (e & 1) res = mul64(res, a, mod);
        a = mul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* simple prime factorisation for 64‑bit values */
static void factor_u64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    uint64_t d = 2;
    while (d <= n / d)
    {
        if (n % d == 0)
        {
            int e = 0;
            while (n % d == 0) { n /= d; ++e; }
            primes[(*cnt)] = d;
            exps[(*cnt)]   = e;
            (*cnt)++;
        }
        d = (d == 2) ? 3 : d + 2;
    }
    if (n > 1)
    {
        primes[(*cnt)] = n;
        exps[(*cnt)]   = 1;
        (*cnt)++;
    }
}

/* integer square root for arbitrary‑precision seq_t using binary search */
static seq_t isqrt_seq(seq_t n)
{
    seq_t low = 0, high = n;
    while (low <= high)
    {
        seq_t mid = (low + high) / 2;
        seq_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else        high = mid - 1;
    }
    return high;
}

static bool is_square(seq_t x)
{
    seq_t r = isqrt_seq(x);
    return r * r == x;
}

/* ---------- main sequence function --------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;            /* obvious for the offset */

    /* factor n */
    uint64_t primes[64];
    int      exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* search for the smallest suitable k */
    seq_t k = (seq_t)2;
    while (1)
    {
        /* skip squares */
        if (is_square(k)) { k++; continue; }

        /* coprime test */
        uint64_t km = (uint64_t)(k % (seq_t)n);
        if (gcd64(km, (uint64_t)n) != 1) { k++; continue; }

        /* quadratic residue test */
        bool qr = true;
        for (size_t i = 0; i < cnt && qr; ++i)
        {
            uint64_t p  = primes[i];
            int      e  = exps[i];
            if (p == 2)
            {
                if (e == 1)
                {
                    if ((k % (seq_t)2) != 1) qr = false;
                }
                else if (e == 2)
                {
                    if ((k % (seq_t)4) != 1) qr = false;
                }
                else            /* e >= 3 */
                {
                    if ((k % (seq_t)8) != 1) qr = false;
                }
            }
            else    /* odd prime */
            {
                uint64_t kmod = (uint64_t)(k % (seq_t)p);
                uint64_t leg  = mod_pow_uint64(kmod, (p - 1) / 2, p);
                if (leg == p - 1) qr = false;   /* Legendre symbol -1 */
            }
        }

        if (qr) return k;

        ++k;
    }
}
