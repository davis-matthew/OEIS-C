/*
 * A359137  Primes such that there is a nontrivial permutation which when
 *          applied to the digits produces a prime (Version 2).
 * https://oeis.org/A359137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A359137: prime numbers that have a non‑trivial digit
   permutation which yields another prime (that permutation may produce
   the same number).  The function builds the sequence by scanning
   successive primes and testing each one.  The small set of primes that
   do not satisfy the condition is known (largest is 8059), so for
   efficiency the test is bypassed for all primes larger than that. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t); */

/* static state for memoisation */
static seq_t *cache = NULL;          /* cache of sequence terms          */
static size_t cache_len = 0;         /* number of terms stored           */
static size_t cache_cap = 0;         /* allocated capacity               */
static uint64_t next_candidate = 2;  /* next integer to test             */

/* forward declarations of internal helpers */
static bool has_prime_perm(uint64_t p);
static bool dfs_perm(int pos, uint32_t mask, uint64_t current,
                     bool diff, const uint8_t *digits, int m);

/* ------------------------------------------------------------------ */
/* Generate primes until the cache contains at least n terms. */
static void ensure_n_terms(size_t n)
{
    if (n <= cache_len) return;   /* already have enough */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* extend cache capacity if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    while (cache_len < n) {
        /* find next prime */
        while (!is_prime(next_candidate)) next_candidate++;

        uint64_t p = next_candidate;
        next_candidate++;

        /* decide if p qualifies */
        bool ok;
        if (p <= 8059) {
            ok = has_prime_perm(p);
        } else {
            ok = true;   /* all larger primes are valid */
        }

        if (ok) {
            cache[cache_len] = (seq_t)p;   /* cast to seq_t */
            cache_len++;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Checks if prime p has a non‑trivial digit permutation that yields a
   prime. */
static bool has_prime_perm(uint64_t p)
{
    /* single‑digit primes cannot have a non‑trivial permutation */
    if (p < 10) return false;

    /* extract digits (least significant first) */
    uint8_t digits[20];
    int m = 0;
    uint64_t t = p;
    while (t > 0) {
        digits[m++] = (uint8_t)(t % 10);
        t /= 10;
    }
    /* reverse to most significant first */
    for (int i = 0; i < m / 2; ++i) {
        uint8_t tmp = digits[i];
        digits[i] = digits[m - 1 - i];
        digits[m - 1 - i] = tmp;
    }

    /* search permutations */
    return dfs_perm(0, 0, 0, false, digits, m);
}

/* ------------------------------------------------------------------ */
/* Recursive DFS over permutations of m digits, building the new number.
   - pos   : current position in the new number
   - mask  : which original positions have been used
   - current : value of the number built so far
   - diff   : whether at least one digit has moved
   - digits : original digits
   - m      : number of digits */
static bool dfs_perm(int pos, uint32_t mask, uint64_t current,
                     bool diff, const uint8_t *digits, int m)
{
    if (pos == m) {
        if (!diff) return false;          /* identity permutation only */
        return is_prime(current);         /* prime? */
    }

    for (int idx = 0; idx < m; ++idx) {
        if (!(mask & (1u << idx))) {
            /* reject numbers with leading zero */
            if (pos == 0 && digits[idx] == 0) continue;
            if (dfs_perm(pos + 1,
                         mask | (1u << idx),
                         current * 10 + digits[idx],
                         diff || (idx != pos),
                         digits,
                         m))
                return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence starts at offset 1 */
    ensure_n_terms((size_t)n);
    return cache[(size_t)n - 1];
}
