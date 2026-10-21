/*
 * A292349  Pri-most primes: primes p such that the majority of bits in the
 *          binary representation of p satisfy the following: complementing
 *          this bit produces a prime number.
 * https://oeis.org/A292349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A292349: Pri-most primes.
   The function a(idx_t n) returns the nth prime such that the
   majority of its binary bits, when flipped, give another prime.
   Implementation uses a simple incremental prime generator (next prime
   via deterministic Miller–Rabin) and caches results for
   repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Helper: 64‑bit deterministic Miller–Rabin primality test.
   ------------------------------------------------------------------ */

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool miller_rabin(uint64_t d, uint64_t n, uint64_t a)
{
    uint64_t x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    while (d != n - 1) {
        x = mul_mod(x, x, n);
        d <<= 1;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t p = 2; p <= 37; ++p) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    while ((d & 1) == 0) d >>= 1;
    /* deterministic bases for 64-bit numbers */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) return true;
        if (!miller_rabin(d, n, a)) return false;
    }
    return true;
}

/* --------------------------------------------------------------
   Bit length of a 64‑bit unsigned integer.
   -------------------------------------------------------------- */
static idx_t bitlen_u64(uint64_t x)
{
    idx_t l = 0;
    while (x) { x >>= 1; ++l; }
    return l;
}

/* --------------------------------------------------------------
   Test whether a 64‑bit prime is a pri‑most prime.
   -------------------------------------------------------------- */
static bool is_pri_most(uint64_t p)
{
    idx_t bl = bitlen_u64(p);
    idx_t threshold = bl / 2;          /* majority requires > threshold */
    idx_t good = 0;
    for (idx_t i = 0; i < bl; ++i) {
        uint64_t mask = 1ULL << i;
        uint64_t q = p ^ mask;
        if (is_prime_u64(q)) {
            ++good;
            if (good > threshold) return true; /* early exit */
        }
    }
    return false;
}

/* --------------------------------------------------------------
   Compute the n-th element of the sequence.
   Caching allows repeated calls in any order.
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, but harmless */

    static seq_t *terms = NULL;
    static size_t term_alloc = 0;
    static idx_t term_count = 0;
    static uint64_t next_prime_val = 2;   /* first prime to consider */

    if (terms == NULL) {
        term_alloc = 128;
        terms = (seq_t *)malloc(term_alloc * sizeof(seq_t));
    }

    while (term_count < n) {
        uint64_t p = next_prime_val;

        /* P must be prime; next_prime_val always holds a prime. */
        if (is_pri_most(p)) {
            if (term_count == term_alloc) {
                term_alloc *= 2;
                terms = (seq_t *)realloc(terms, term_alloc * sizeof(seq_t));
            }
            terms[term_count] = (seq_t)p;
            ++term_count;
        }

        /* Find next prime > p */
        uint64_t q = p + 1;
        while (!is_prime_u64(q)) ++q;
        next_prime_val = q;
    }

    return terms[n - 1];
}
