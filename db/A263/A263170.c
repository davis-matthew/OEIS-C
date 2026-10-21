/*
 * A263170  a(n) = (Sum_{k=1..n} prime(k))^3 - (Sum_{k=1..n} prime(k)^3).
 * https://oeis.org/A263170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Sum_{k=1..n} prime(k))^3 - Sum_{k=1..n} prime(k)^3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 30, u64 n <= 857,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static prime storage */
static uint64_t *primes      = NULL;   /* array of primes */
static size_t   primes_cap  = 0;
static size_t   primes_cnt  = 0;

/* static prefix sums:
   sum_prime_pref[i]  = Σ_{k=1..i+1} prime(k)
   sum_cubes_pref[i]  = Σ_{k=1..i+1} prime(k)^3
   (i is 0‑based) */
static seq_t   *sum_prime_pref = NULL;
static seq_t   *sum_cubes_pref = NULL;
static size_t  pref_cap = 0;
static size_t  pref_cnt = 0;

/* largest n processed so far */
static idx_t max_n_done = 0;

/* grow the primes array to hold at least 'need' entries */
static void grow_primes(size_t need)
{
    if (need <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* grow the prefix sum arrays to hold at least 'need' entries */
static void grow_pref(size_t need)
{
    if (need <= pref_cap) return;
    size_t new_cap = pref_cap ? pref_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp1 = (seq_t *)realloc(sum_prime_pref, new_cap * sizeof(seq_t));
    if (!tmp1) abort();
    sum_prime_pref = tmp1;
    seq_t *tmp2 = (seq_t *)realloc(sum_cubes_pref, new_cap * sizeof(seq_t));
    if (!tmp2) abort();
    sum_cubes_pref = tmp2;
    pref_cap = new_cap;
}

/* test whether 'cand' is prime using the primes already known */
static bool is_prime_candidate(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        unsigned __int128 sq = (unsigned __int128)p * p;
        if (sq > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* generate primes and prefix sums up to at least 'target' primes */
static void generate_up_to(idx_t target)
{
    if (target <= max_n_done) return;
    grow_primes((size_t)target);
    grow_pref((size_t)target);

    uint64_t candidate = (primes_cnt == 0) ? 2
                                            : primes[primes_cnt - 1] + 1;

    while (primes_cnt < (size_t)target) {
        /* find next prime */
        while (!is_prime_candidate(candidate)) ++candidate;

        /* store prime */
        primes[primes_cnt] = candidate;

        /* update prefix sums */
        seq_t prev_sum  = (primes_cnt == 0) ? 0 : sum_prime_pref[primes_cnt - 1];
        seq_t sum       = prev_sum + (seq_t)candidate;
        sum_prime_pref[primes_cnt] = sum;

        seq_t prev_cube = (primes_cnt == 0) ? 0 : sum_cubes_pref[primes_cnt - 1];
        seq_t cube      = (seq_t)candidate * (seq_t)candidate * (seq_t)candidate;
        sum_cubes_pref[primes_cnt] = prev_cube + cube;

        ++primes_cnt;
        ++candidate;
    }

    max_n_done = target;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* offset 1 */
    generate_up_to(n);
    seq_t sum       = sum_prime_pref[n - 1];
    seq_t cube_sum  = sum_cubes_pref[n - 1];
    seq_t cube      = sum * sum * sum;
    return cube - cube_sum;
}
