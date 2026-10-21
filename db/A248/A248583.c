/*
 * A248583  The least number m == 1 (mod 6) that is divisible by prime(n).
 * https://oeis.org/A248583
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A248583 – a(n) = prime(n) if prime(n) ≡ 1 (mod 6),
    otherwise 5·prime(n).                                 */
/*  Indexing follows OEIS: a(3) is the first listed term.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Prime list storage                                               */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* primes in increasing order   */
static size_t   prime_count   = 0;      /* number of primes stored      */
static size_t   prime_cap     = 0;      /* allocated capacity          */

/* ------------------------------------------------------------------ */
/*  Grow the prime list to accommodate at least 'new_cap' entries.    */
/* ------------------------------------------------------------------ */
static void grow_prime_list(size_t new_cap)
{
    if (prime_cap >= new_cap) return;
    size_t cap = prime_cap ? prime_cap * 2 : 16;
    if (cap < new_cap) cap = new_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
    if (!tmp) abort();                /* allocation failure */
    prime_list   = tmp;
    prime_cap    = cap;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the prime list contains at least the prime with        */
/*  zero‑based index 'need' (i.e. prime(need+1)).                       */
/* ------------------------------------------------------------------ */
static void ensure_prime_index(idx_t need)
{
    /* fast exit if we already have enough primes */
    if (prime_count > need) return;

    /* initialise the list with the first two primes if empty */
    if (prime_count == 0) {
        grow_prime_list(2);
        prime_list[prime_count++] = 2;
        prime_list[prime_count++] = 3;
        if (prime_count > need) return;
    }

    /* start searching from the next odd integer after the last prime */
    uint64_t cand;
    if (prime_count == 1)            /* only 2 stored so far */
        cand = 3;
    else
        cand = prime_list[prime_count - 1] + 2; /* last prime is odd */

    while (prime_count <= need) {
        bool is_prime = true;

        /* trial division by primes already found */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;            /* p > sqrt(cand)   */
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            grow_prime_list(prime_count + 1);
            prime_list[prime_count++] = cand;
            if (prime_count > need) break;
        }
        cand += 2;            /* skip even numbers */
    }
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* below offset – value irrelevant */

    /* prime(n) has zero‑based index (n‑1) in our list */
    idx_t idx = n - 1;
    ensure_prime_index(idx);

    seq_t p      = (seq_t)prime_list[idx];
    seq_t mod6   = p % (seq_t)6;
    seq_t mult   = (mod6 == (seq_t)1) ? (seq_t)1 : (seq_t)5;

    return p * mult;
}
