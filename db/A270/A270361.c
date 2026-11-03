/*
 * A270361  Odd primes p for which there exists an odd prime q < p such that
 *          p*q - 1 is a square.
 * https://oeis.org/A270361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 141, u64 n <= 10471,
 *            u128 n <= 10471, big any n
 * Validated: standard, pass, 62 terms through n = 9631 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A270361: odd primes p for which there exists an odd prime q<p
   such that p*q-1 is a perfect square.  The algorithm exploits that
   p≡1 (mod 4) and that for such a prime a √(-1) exists.  For each p,
   if r is a square root of -1 modulo p, then all squares of the form
   (r+m p)²+1 are multiples of p; writing (r+m p)²+1=p⋅q gives
   q=t+2r m+m²p.  Since q<p implies m=0, we only need to test
   q=t=(r²+1)/p for the two possible square roots r and p−r.  If
   either t is an odd prime smaller than p, then p belongs to the
   sequence.  This test is performed for successive primes until the
   requested index is reached.  The sequence is stored in a static
   cache so that repeated calls are fast.  */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
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

/* ------------------------------------------------------------------ */
/* Compute a square root of -1 modulo an odd prime p (p%4==1).          */
/* ------------------------------------------------------------------ */
static seq_t tonelli_shanks_minus_one(uint64_t p)
{
    const seq_t P   = (seq_t)p;
    const seq_t minus_one = P - 1;          /* a = -1 mod p */

    /* Factor p-1 as Q * 2^S with Q odd. */
    uint64_t Q = p - 1;
    uint64_t S = 0;
    while ((Q & 1ULL) == 0) {
        Q >>= 1;
        ++S;
    }

    /* Find a quadratic non‑residue z. */
    uint64_t z = 2;
    seq_t z_mod = (seq_t)z;
    while (true) {
        seq_t ls = powmod(z_mod, (p - 1) / 2, P);   /* Jacobi symbol */
        if (ls == P - 1) break;                      /* Non‑residue */
        ++z;
        z_mod = (seq_t)z;
    }

    /* Initialize Tonelli–Shanks variables. */
    seq_t c = powmod(z_mod, Q, P);
    seq_t t = powmod(minus_one, Q, P);
    seq_t r = powmod(minus_one, (Q + 1) / 2, P);

    /* Main loop. */
    while (t != (seq_t)1) {
        uint64_t i = 1;
        seq_t t2 = (t * t) % P;
        while (t2 != (seq_t)1) {
            t2 = (t2 * t2) % P;
            ++i;
        }
        seq_t b = powmod(c, 1ULL << (S - i - 1), P);
        r = (r * b) % P;
        t = (t * b % P) * b % P;
        c = (b * b) % P;
        S = i;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine.                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   len = 0;
    static size_t   cap = 0;
    static uint64_t last_checked = 2;     /* start after 2 */

    while (len < n) {
        /* Find next prime > last_checked. */
        do { ++last_checked; } while (!is_prime(last_checked));

        /* p must be ≡1 (mod 4). */
        if ((last_checked & 3ULL) != 1ULL) continue;

        uint64_t p = last_checked;
        const seq_t P = (seq_t)p;

        /* One square root of -1 modulo p. */
        seq_t r = tonelli_shanks_minus_one(p);

        /* Check the two possible quotients t. */
        bool ok = false;
        for (int root_iter = 0; root_iter < 2 && !ok; ++root_iter) {
            seq_t r_cur = (root_iter == 0) ? r : (P - r);
            seq_t numer = (r_cur * r_cur) + (seq_t)1;
            seq_t t = numer / P;          /* integer quotient */

            if (t > (seq_t)2 && (t & (seq_t)1) &&          /* odd prime > 2 */
                t < P && is_prime_big(t)) {
                ok = true;
            }
        }

        if (ok) {
            /* Grow cache if necessary. */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = P;
        }
    }

    return cache[n - 1];
}
