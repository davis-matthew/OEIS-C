/*
 * A307152  a(n) = floor((A002144(n)+19)/24).
 * https://oeis.org/A307152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (A002144(n) + 19) / 24 ) where A002144(n) = the
 *            n‑th prime congruent to 1 (mod 4)
 * Range:     u8 n <= 24, u16 n <= 4222, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Prime storage for all primes found so far. */
static seq_t *primes = NULL;      /* array of all primes */
static size_t primes_cnt = 0;     /* number of primes stored */
static size_t primes_cap = 0;     /* capacity of primes array */

/* Prime storage for primes ≡ 1 (mod 4). */
static seq_t *primes1 = NULL;     /* array of primes ≡ 1 (mod 4) */
static size_t primes1_cnt = 0;    /* number of such primes stored */
static size_t primes1_cap = 0;    /* capacity of primes1 array */

/* Next odd integer candidate to test for primality. */
static idx_t current_candidate = 5;

/* Ensure primes array has at least `need` slots. */
static void ensure_primes_cap(size_t need) {
    if (need > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp && new_cap) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* Ensure primes1 array has at least `need` slots. */
static void ensure_primes1_cap(size_t need) {
    if (need > primes1_cap) {
        size_t new_cap = primes1_cap ? primes1_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(primes1, new_cap * sizeof(seq_t));
        if (!tmp && new_cap) abort();
        primes1 = tmp;
        primes1_cap = new_cap;
    }
}

/* Generate primes until at least `target` primes ≡ 1 (mod 4) are found. */
static void generate_primes_until(size_t target) {
    /* First call: initialise prime tables. */
    if (primes == NULL) {
        primes_cap = 64;
        primes = (seq_t*)malloc(primes_cap * sizeof(seq_t));
        if (!primes) abort();
        primes[0] = (seq_t)2;
        primes[1] = (seq_t)3;
        primes_cnt = 2;

        primes1_cap = 64;
        primes1 = (seq_t*)malloc(primes1_cap * sizeof(seq_t));
        if (!primes1) abort();
        primes1_cnt = 0;
    }

    while (primes1_cnt < target) {
        idx_t cand = current_candidate;
        current_candidate += 2;                /* test only odd numbers */

        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            idx_t p_idx = (idx_t)p;
            if (p_idx * p_idx > cand) break;   /* sqrt check */
            if (cand % p_idx == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store new prime in the general primes list. */
            ensure_primes_cap(primes_cnt + 1);
            primes[primes_cnt++] = (seq_t)cand;

            /* If it is ≡ 1 (mod 4), store it in primes1 list. */
            if ((cand & 3) == 1) {
                ensure_primes1_cap(primes1_cnt + 1);
                primes1[primes1_cnt++] = (seq_t)cand;
            }
        }
    }
}

/* External function required by the benchmark. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* out of range, safe fallback */
    generate_primes_until((size_t)n);
    seq_t p1 = primes1[n - 1];            /* nth prime ≡ 1 (mod 4) */
    return (p1 + (seq_t)19) / (seq_t)24;  /* integer division = floor */
}
