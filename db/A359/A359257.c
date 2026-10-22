/*
 * A359257  First differences of A002476.
 * https://oeis.org/A359257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A359257: a(n) = P_{n+1} – P_n where P_k is the k‑th prime ≡ 1 (mod 6). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- integer square root (floor) ---------- */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; i--) {
        uint64_t cand = r | ((uint64_t)1 << i);
        /* use 128‑bit to avoid overflow for large cand */
        if ((unsigned __int128)cand * cand <= x)
            r = cand;
    }
    return r;
}

/* ---------- static storage for primes ---------- */
static uint64_t *all_primes   = NULL;   /* all primes found so far */
static size_t   all_cnt      = 0;        /* current number of primes */
static size_t   all_cap      = 0;        /* capacity of all_primes */

static uint64_t *mod1_primes = NULL;   /* primes ≡ 1 (mod 6) */
static size_t   mod1_cnt    = 0;        /* number of such primes */
static size_t   mod1_cap    = 0;        /* capacity of mod1_primes */

static uint64_t next_candidate = 3;     /* smallest odd number to test */

/* ---------- capacity helpers ---------- */
static void ensure_all_cap(void)
{
    if (all_cnt == all_cap) {
        size_t new_cap = all_cap ? 2 * all_cap : 16;
        all_primes = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
        if (!all_primes) exit(EXIT_FAILURE);
        all_cap = new_cap;
    }
}

static void ensure_mod1_cap(void)
{
    if (mod1_cnt == mod1_cap) {
        size_t new_cap = mod1_cap ? 2 * mod1_cap : 8;
        mod1_primes = (uint64_t *)realloc(mod1_primes, new_cap * sizeof(uint64_t));
        if (!mod1_primes) exit(EXIT_FAILURE);
        mod1_cap = new_cap;
    }
}

/* ---------- primality test ---------- */
static bool is_prime(uint64_t cand)
{
    uint64_t lim = isqrt_u64(cand);
    for (size_t i = 0; i < all_cnt && all_primes[i] <= lim; i++) {
        if (cand % all_primes[i] == 0)
            return false;
    }
    return true;
}

/* ---------- generate enough 6k+1 primes ---------- */
static void ensure_mod1_primes(idx_t n) /* need indices 0..n inclusive */
{
    while (mod1_cnt <= (size_t)n) {
        uint64_t cand = next_candidate;
        if (is_prime(cand)) {
            ensure_all_cap();
            all_primes[all_cnt++] = cand;

            if (cand % 6 == 1) {
                ensure_mod1_cap();
                mod1_primes[mod1_cnt++] = cand;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1, harmless */

    /* initialise if first call */
    if (!all_primes) {
        all_cap = 16;
        all_primes = (uint64_t *)malloc(all_cap * sizeof(uint64_t));
        if (!all_primes) exit(EXIT_FAILURE);
        all_primes[all_cnt++] = 2;   /* the prime 2 */
    }

    ensure_mod1_primes(n);

    /* difference between (n+1)-th and n-th 6k+1 primes
       (indices are 0‑based in mod1_primes) */
    uint64_t diff = mod1_primes[(size_t)n] - mod1_primes[(size_t)n - 1];
    return (seq_t)diff;
}
