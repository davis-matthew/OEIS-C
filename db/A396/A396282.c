/*
 * A396282  Primes q such that q - p = floor(log_2(q)), where p is the largest
 *          prime strictly less than q.
 * https://oeis.org/A396282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A396282:
 * a(n) = nth prime q such that q - p = ⌊log₂ q⌋,
 * where p is the largest prime strictly less than q.
 * OEIS offset = 1.  The function is memoised so that each call
 * requires only O(1) time after the sequence has been generated
 * up to the requested index. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- small helpers -------------------------------------------------- */

/* deterministic Miller‑Rabin for 64‑bit integers (works for 0 < n < 2⁶⁴) */
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t r = (__uint128_t)a * b;
    return (uint64_t)(r % mod);
}

static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1) res = mod_mul64(res, a, mod);
        a = mod_mul64(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n % 2 == 0)     return n == 2;
    if (n % 3 == 0)     return n == 3;
    if (n % 5 == 0)     return n == 5;          /* small primes pre‑tested */

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* floor(log₂ x) for 0 < x < 2⁶⁴ */
static unsigned floor_log2_uint64(uint64_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* ----- memoised state ----------------------------------------------- */

static seq_t *g_terms          = NULL;   /* array of found terms          */
static size_t g_terms_len      = 0;      /* number of stored terms        */
static size_t g_terms_cap      = 0;      /* capacity of g_terms           */
static uint64_t g_last_prime   = 2;      /* last prime processed          */
static bool g_initialized      = false;  /* has static state been set?    */

/* push a new term onto the dynamic array */
static void g_push_term(uint64_t val)
{
    if (g_terms_len == g_terms_cap) {
        g_terms_cap = g_terms_cap ? g_terms_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(g_terms, g_terms_cap * sizeof(seq_t));
        if (!tmp) exit(1);                    /* allocation failed */
        g_terms = tmp;
    }
    g_terms[g_terms_len++] = (seq_t)val;
}

/* find next prime after n (n ≥ 2) */
static uint64_t g_next_prime(uint64_t n)
{
    if (n < 2) return 2;
    uint64_t candidate = n + 1;
    if ((candidate & 1) == 0) ++candidate;           /* make odd */
    for (;; candidate += 2) {
        if (is_prime_uint64(candidate)) return candidate;
    }
}

/* ----- public function ----------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                          /* undefined for n < 1 */

    /* initialise static state on first call */
    if (!g_initialized) {
        g_initialized = true;
    }

    while (g_terms_len < (size_t)n) {
        uint64_t q = g_next_prime(g_last_prime);   /* next prime > last */
        uint64_t gap = q - g_last_prime;
        unsigned log2q = floor_log2_uint64(q);     /* ⌊log₂ q⌋ */

        if (gap == log2q) g_push_term(q);

        g_last_prime = q;                          /* move to next q */
    }

    return g_terms[(size_t)n - 1];
}
