/*
 * A309580  Primes p with 1 zero in a fundamental period of A000129 mod p.
 * https://oeis.org/A309580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1280, u32 n <= 1280, u64 n <= 1280,
 *            u128 n <= 1280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309580 – primes p with 1 zero in a fundamental period of A000129 mod p.
   Computed by iterating primes and testing the condition:
   * p = 2, or
   * p ≡ 7 (mod 8), or
   * p ≡ 1 (mod 8) and the multiplicative order of -(3+2√2) modulo p
     satisfies ord ≡ 2 (mod 4). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------------------------------------------------------------------- */
/*  Algebras in ℤ[√2] modulo a prime p  (needed for order calculation)    */
/* ---------------------------------------------------------------------- */
typedef struct { uint64_t a, b; } pair_t;

/* (x.a + x.b√2)(y.a + y.b√2)  mod p  */
static inline pair_t pair_mul(uint64_t p, pair_t x, pair_t y)
{
    __uint128_t t1 = (__uint128_t)x.a * y.a + (__uint128_t)2 * x.b * y.b;
    __uint128_t t2 = (__uint128_t)x.a * y.b + (__uint128_t)x.b * y.a;
    pair_t r;
    r.a = (uint64_t)(t1 % p);
    r.b = (uint64_t)(t2 % p);
    return r;
}

/* (base)^exp  in ℤ[√2]  modulo p  */
static inline pair_t pair_pow(uint64_t p, pair_t base, uint64_t exp)
{
    pair_t res = {1, 0};
    pair_t cur = base;
    while (exp) {
        if (exp & 1) res = pair_mul(p, res, cur);
        cur = pair_mul(p, cur, cur);
        exp >>= 1;
    }
    return res;
}

/* Multiplicative order of -(3+2√2) modulo p */
static uint64_t order_of_elem(uint64_t p)
{
    /* r = -(3 + 2√2)  ≡ (p-3, p-2) */
    pair_t r = { (p + p - 3) % p, (p + p - 2) % p };

    uint64_t ord = p - 1;          /* φ(p)  */
    uint64_t n = ord;

    for (uint64_t f = 2; f * f <= n; ++f) {
        if (n % f == 0) {
            while (n % f == 0) n /= f;      /* remove prime factor f completely   */
            while (ord % f == 0) {
                uint64_t cand = ord / f;
                pair_t pw = pair_pow(p, r, cand);
                if (pw.a == 1 && pw.b == 0)
                    ord = cand;
                else
                    break;
            }
        }
    }
    if (n > 1) { /* remaining prime factor */
        uint64_t f = n;
        while (ord % f == 0) {
            uint64_t cand = ord / f;
            pair_t pw = pair_pow(p, r, cand);
            if (pw.a == 1 && pw.b == 0)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

/* ---------------------------------------------------------------------- */
/*  Incremental prime generator (trial division)                          */
/* ---------------------------------------------------------------------- */
static uint64_t *prime_gen = NULL;        /* list of primes found so far      */
static size_t prime_gen_cnt = 0;
static size_t prime_gen_cap = 0;
static uint64_t last_candidate = 1;

/* Grow the storage for prime_gen   */
static void grow_prime_gen(void)
{
    prime_gen_cap = (prime_gen_cap == 0) ? 10 : prime_gen_cap * 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_gen, prime_gen_cap * sizeof(uint64_t));
    if (tmp) prime_gen = tmp;
}

/* Return the next prime number (incremental) */
static uint64_t next_prime(void)
{
    if (prime_gen_cnt == 0) { /* initialise with 2 */
        grow_prime_gen();
        prime_gen[prime_gen_cnt++] = 2;
        last_candidate = 2;
        return 2;
    }

    for (;;) {
        ++last_candidate;
        if (last_candidate % 2 == 0) continue;          /* skip even numbers */
        uint64_t limit = (uint64_t)sqrt((double)last_candidate);
        bool is_p = true;
        for (size_t i = 0; i < prime_gen_cnt && prime_gen[i] <= limit; ++i) {
            if (last_candidate % prime_gen[i] == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_gen_cnt == prime_gen_cap) grow_prime_gen();
            prime_gen[prime_gen_cnt++] = last_candidate;
            return last_candidate;
        }
    }
}

/* ---------------------------------------------------------------------- */
/*  Sequence storage (memoised)                                            */
/* ---------------------------------------------------------------------- */
static seq_t *seq_vals = NULL;      /* cached terms of the sequence */
static size_t seq_cnt  = 0;          /* number of cached terms       */
static size_t seq_cap  = 0;

/* Append a new term to the cache */
static void seq_append(seq_t val)
{
    if (seq_cnt == seq_cap) {
        seq_cap = (seq_cap == 0) ? 10 : seq_cap * 2;
        seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
    }
    seq_vals[seq_cnt++] = val;
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function                                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour for n<1 is irrelevant */

    /* Initialise cache with first term if empty */
    if (seq_cnt == 0) seq_append((seq_t)2);

    /* Generate terms until we reach the requested index */
    while (seq_cnt < (size_t)n) {
        uint64_t p = next_prime();
        if (p == 2) continue; /* already cached */

        if (p % 8 == 7) {
            seq_append((seq_t)p);
        } else if (p % 8 == 1) {
            uint64_t ord = order_of_elem(p);
            if (ord % 4 == 2) {
                seq_append((seq_t)p);
            }
        }
    }
    return seq_vals[n - 1];
}
