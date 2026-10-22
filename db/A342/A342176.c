/*
 * A342176  Tower of primes modulo n: a(n) = (2^3^5^7^ ... ^prime(n)) mod n.
 * https://oeis.org/A342176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^3^5^...^prime(n)) mod n
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   build_primes_upto, calc_phi, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"
#include "../utils/calc_phi.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* 0‑based array of primes      */
static size_t   prime_cnt   = 0;      /* number of primes currently  */
static idx_t    prime_limit = 200;    /* initial sieve limit          */

/* Ensure that prime_list contains at least n primes.  */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n) return;

    while (prime_cnt < n) {
        if (prime_list) free(prime_list);

        build_primes_upto(prime_limit, &prime_list, &prime_cnt);

        if (prime_cnt >= n) break;
        prime_limit <<= 1;               /* double the limit for next try */
    }
}

static idx_t current_n;   /* holds the n passed to a() */

/* Recursive evaluation of the prime power tower.
   idx   – index in prime_list (1‑based)
   mod   – modulus under which current level is evaluated
   res   – pointer to receive the result modulo mod
   larg  – pointer to flag: true if the real value of the tower at this
           level is ≥ mod (used by the parent to decide exponent reduction) */
static void tower_rec(idx_t idx, seq_t mod, seq_t *res, bool *larg)
{
    if (mod == (seq_t)1) {
        *res = (seq_t)0;
        *larg = true;                   /* any tower value ≥ 1 */
        return;
    }

    if (idx == current_n) {
        seq_t val = (seq_t)prime_list[idx - 1];
        *res = val % mod;
        *larg = (val >= mod);           /* true if tower itself ≥ mod */
        return;
    }

    seq_t phi_mod = calc_phi(mod);
    seq_t child_val;
    bool child_larg;
    tower_rec(idx + 1, phi_mod, &child_val, &child_larg);

    seq_t exponent = child_val + (child_larg ? phi_mod : (seq_t)0);
    /* exponent fits in uint64_t because exponent < 2*phi_mod ≤ 2*mod ≤ 2*n */
    *res = mod_pow((seq_t)prime_list[idx - 1], (uint64_t)exponent, mod);

    /* For mod ≥ 2 the tower is always ≥ mod, so set larg to true.
       This guarantees that at a higher level the exponent will be
       reduced correctly (if needed). */
    *larg = true;
}

/* Main sequence function: a(n) = (2^3^5^…^prime(n)) mod n. */
seq_t a(idx_t n)
{
    /* Prepare primes up to n */
    ensure_primes(n);

    /* a(1) is 0 because any number mod 1 is 0. */
    if (n == 1) return (seq_t)0;

    /* Any power of two returns 0 – matches the OEIS definition. */
    if ((n & (n - 1)) == 0) return (seq_t)0;

    current_n = n;
    seq_t out;
    bool dummy;
    tower_rec((idx_t)1, (seq_t)n, &out, &dummy);
    return out;
}
