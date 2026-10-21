/*
 * A275682  Table read by rows: list of sexy prime quadruples (p, p+6, p+12,
 *          p+18) such that p+24 is composite.
 * https://oeis.org/A275682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 216, u32 n <= 2100, u64 n <= 2100,
 *            u128 n <= 2100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275682: list of primes in sexy prime quadruples (p, p+6, p+12, p+18) such that p+24 is composite.
   The sequence is produced by brute‑force search over odd candidate primes.
   Each qualifying quadruple contributes its four primes in order to the sequence. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper functions for 64‑bit primality test ---------- */

/* 64‑bit modular multiplication using 128‑bit intermediate (works for all 64‑bit values). */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* 64‑bit modular exponentiation. */
static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1U) result = mul_mod(result, cur, mod);
        cur = mul_mod(cur, cur, mod);
        exp >>= 1U;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit integers. */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* Small prime trial division */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
        31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,
        71ULL,73ULL,79ULL,83ULL,89ULL,97ULL,101ULL,103ULL,107ULL,
        109ULL,113ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller–Rabin with bases sufficient for all 64‑bit numbers. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }
    static const uint64_t bases[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1U || x == n - 1U) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1U) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static inline bool is_prime(idx_t p)   { return is_prime_u64((uint64_t)p); }
static inline bool is_composite(idx_t p){ return !is_prime(p); }

/* ---------- Sequence storage and helper ---------- */

static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* Grow the internal array to at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    if (seq_cap == 0) seq_cap = 16;
    while (seq_cap < need) seq_cap <<= 1U;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
}

/* ---------- Main function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1, guard against zero */

    /* If we already have enough precomputed values, return directly. */
    if (seq_len >= (size_t)n) return seq[n - 1];

    static idx_t last_checked = 5;            /* start scanning from 5 */

    while (seq_len < (size_t)n) {
        /* Search for the next qualifying prime 'p'. */
        idx_t p = last_checked;
        if (p < 5) p = 5;
        if ((p & 1U) == 0) ++p;               /* only odd candidates */

        while (true) {
            if (is_prime(p) &&
                is_prime(p + 6) &&
                is_prime(p + 12) &&
                is_prime(p + 18) &&
                !is_prime(p + 24)) {

                /* Store the four primes of the quadruple. */
                ensure_seq_cap(seq_len + 4);
                seq[seq_len]     = (seq_t)p;
                seq[seq_len + 1] = (seq_t)(p + 6);
                seq[seq_len + 2] = (seq_t)(p + 12);
                seq[seq_len + 3] = (seq_t)(p + 18);
                seq_len += 4;
                break;
            }
            p += 2;                           /* next odd candidate */
        }

        last_checked = p + 2;                 /* skip past current 'p' */
    }

    return seq[n - 1];
}
