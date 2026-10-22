/*
 * A308790  Primes p such that A001175(p) = (p-1)/5.
 * https://oeis.org/A308790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factor_u64, is_prime, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A308790: Primes p such that ord((1+√5)/2 , p) = (p-1)/5
    The equivalence is:  p ≡ 1 (mod 5) and the Pisano period of the
    Fibonacci sequence modulo p equals (p-1)/5.
    The code sieves primes, tests the period condition via fast‑doubling
    Fibonacci modulo arithmetic and memoises the found primes so that
    a(n) can be returned in O(1).  */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime.h"      /* bool is_prime(idx_t x);        */
#include "../utils/factor_u64.h"    /* size_t factor_u64(...);        */
#include "../utils/mod_mul.h"      /* uint64_t mod_mul(...);         */

/* Fast‑doubling Fibonacci modulo 'mod':
   returns Fn   = F_n   (mod mod)
          Fn1  = F_{n+1} (mod mod) */
static void fib_mod_pair(uint64_t n, uint64_t mod,
                         uint64_t *Fn, uint64_t *Fn1)
{
    if (n == 0) {
        *Fn  = 0;
        *Fn1 = 1;
        return;
    }
    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);

    /* c = a * (2*b - a) mod mod */
    uint64_t two_b = mod_mul(b, 2, mod);
    uint64_t temp   = (two_b + mod - a) % mod;          /* (2*b - a) mod */
    uint64_t c      = mod_mul(a, temp, mod);

    /* d = a*a + b*b mod mod */
    uint64_t aa = mod_mul(a, a, mod);
    uint64_t bb = mod_mul(b, b, mod);
    uint64_t d  = (aa + bb) % mod;

    if (n & 1) {                    /* n odd */
        *Fn  = d;
        *Fn1 = (c + d) % mod;
    } else {                        /* n even */
        *Fn  = c;
        *Fn1 = d;
    }
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t next_candidate = 2;   /* start with smallest prime */

    if (n == 0) return (seq_t)0;   /* offset is 1; behaviour for 0 is arbitrary */

    while (seq_len < (size_t)n) {
        uint64_t p = next_candidate;

        /* Find next prime candidate */
        for (;;) {
            if (p == 2) /* 2 is prime */
                break;
            if (is_prime((idx_t)p))
                break;
            p += 2;  /* skip even numbers */
        }

        /* Prepare subsequent candidate */
        next_candidate = (p == 2 ? 3 : p + 2);

        /* p must satisfy p ≡ 1 (mod 5) */
        if ((p % 5ull) != 1)
            continue;

        uint64_t d = (p - 1) / 5;

        /* Check that Pisano period divides d:
           that is, F_d ≡ 0 and F_{d+1} ≡ 1 (mod p) */
        uint64_t fd, fd1;
        fib_mod_pair(d, p, &fd, &fd1);
        if (!(fd == 0 && fd1 == 1))
            continue;

        /* Ensure no proper divisor of d gives the same condition.
           A divisor t of d is a period iff F_t ≡ 0 and F_{t+1} ≡ 1.
           It suffices to test t = d / q for each prime factor q of d. */
        uint64_t pf[64], ex[64];
        size_t npf = 0;
        factor_u64(d, pf, ex, &npf);
        bool reject = false;
        for (size_t i = 0; i < npf && !reject; ++i) {
            uint64_t t = d / pf[i];
            uint64_t ft, ft1;
            fib_mod_pair(t, p, &ft, &ft1);
            if (ft == 0 && ft1 == 1)
                reject = true;
        }
        if (reject)
            continue;

        /* Accept the prime */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)p;
    }

    return seq[n - 1];
}
