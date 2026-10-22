/*
 * A369150  Numbers k neither squarefree nor prime powers such that A053669(k)
 *          < k/rad(k) < A119288(k) that are not odd numbers of the form
 *          lpf(k)*rad(k), where lpf(k) = A020639(k) and rad(k) = A007947(k).
 * https://oeis.org/A369150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 4865, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------
   OEIS A369150
   Numbers k neither squarefree nor prime powers such that
   A053669(k) < k/rad(k) < A119288(k) and k/rad(k) != A020639(k).
   The function searches forward, storing terms in a static buffer
   (sieve-like).  No hard‑coded table, works for any seq_t type.
-----------------------------------------------------------------*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Prime table (generated once)
   ------------------------------------------------------------------ */
static uint32_t *prime_tbl   = NULL;   /* array of primes      */
static size_t    prime_cnt   = 0;      /* number of primes     */
static bool      primes_inited = false;

static void init_primes (void)
{
    if (primes_inited) return;

    const uint32_t LIM = 200000;          /* enough for √(k) up to ~4·10¹⁰   */
    bool *comp = (bool *)calloc((size_t)LIM + 1, sizeof(bool));
    if (!comp) return;

    uint32_t *tbl = (uint32_t *)malloc((size_t)LIM * sizeof(uint32_t));
    size_t cnt = 0;

    for (uint32_t i = 2; i <= LIM; ++i) {
        if (!comp[i]) {
            tbl[cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)LIM) {
                for (uint64_t j = (uint64_t)i * i; j <= (uint64_t)LIM; j += i) {
                    comp[(size_t)j] = true;
                }
            }
        }
    }
    prime_tbl = (uint32_t *)realloc(tbl, cnt * sizeof(uint32_t));
    prime_cnt = cnt;
    free(comp);
    primes_inited = true;
}

/* ------------------------------------------------------------------
   Check whether a given k satisfies the conditions of A369150
   ------------------------------------------------------------------ */
static bool is_term (uint64_t k)
{
    /* Sequence never contains multiples of 6 – quick rejection. */
    if (((k & 1ULL) == 0ULL) && (k % 3ULL == 0ULL)) return false;

    init_primes();

    uint64_t temp = k;
    uint64_t rad   = 1ULL;   /* product of distinct primes   */
    uint64_t s     = 1ULL;   /* k / rad(k)                   */
    uint64_t first = 0ULL;   /* least prime factor           */
    uint64_t second = 0ULL;  /* second distinct prime factor */
    bool has_square = false;
    size_t dist_cnt = 0;

    /* handle factor 2 */
    int exp2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        ++exp2;
    }
    if (exp2 > 0) {
        ++dist_cnt;
        if (exp2 > 1) has_square = true;
        rad *= 2ULL;
        if (exp2 > 1) {
            uint64_t pow = 1ULL << (exp2 - 1);   /* 2^(exp2-1) */
            s *= pow;
        }
        first = 2ULL;
    }

    /* odd primes ---------------------------------------------- */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_tbl[i] * (uint64_t)prime_tbl[i] <= temp; ++i) {
        uint32_t p = prime_tbl[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            ++dist_cnt;
            if (exp > 1) has_square = true;
            rad *= (uint64_t)p;
            if (exp > 1) {
                uint64_t pow = 1ULL;
                for (int j = 1; j < exp; ++j) pow *= (uint64_t)p;
                s *= pow;
            }
            if (first == 0ULL) first = p;
            else if (second == 0ULL) second = p;
        }
    }

    /* remaining prime (if any) ------------------------------ */
    if (temp > 1ULL) {
        ++dist_cnt;
        rad *= temp;
        if (first == 0ULL) first = temp;
        else if (second == 0ULL) second = temp;
    }

    /* must have at least two distinct primes and a squared factor */
    if (dist_cnt < 2) return false;
    if (!has_square) return false;

    /* k/rad(k) must differ from the least prime factor */
    if (s == first) return false;

    /* smallest prime not dividing k */
    uint64_t q = ((k & 1ULL) == 0ULL) ? 3ULL : 2ULL;

    /* inequality condition */
    return (q < s) && (s < second);
}

/* ------------------------------------------------------------------
   Storage for generated terms
   ------------------------------------------------------------------ */
static uint64_t *term_buf  = NULL;   /* 1‑based array of found terms */
static size_t   term_len  = 0;       /* current number of terms stored */
static size_t   term_cap  = 0;       /* allocated capacity */
static uint64_t next_k    = 2ULL;    /* next candidate to test */

/* ------------------------------------------------------------------
   a(n) – Return the n-th term (n >= 1)
   ------------------------------------------------------------------ */
seq_t a (idx_t n)
{
    /* initialise buffer */
    if (term_len == 0) {
        term_cap = 64;
        term_buf = (uint64_t *)malloc(term_cap * sizeof(uint64_t));
        term_buf[0] = 0;   /* dummy to make indices 1‑based */
        term_len = 1;
    }

    /* generate terms until we have at least n terms */
    while (term_len <= (size_t)n) {
        while (!is_term(next_k)) ++next_k;

        if (term_len == term_cap) {
            term_cap <<= 1;
            term_buf = (uint64_t *)realloc(term_buf, term_cap * sizeof(uint64_t));
        }
        term_buf[term_len++] = next_k;
        ++next_k;
    }

    return (seq_t)term_buf[(size_t)n];
}
