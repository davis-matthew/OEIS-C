/*
 * A382659  Numbers k such that k < A053669(k)^2 * A380539(k), i.e., k <
 *          A382248(k).
 * https://oeis.org/A382659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: full, pass, 72 terms through n = 72 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A382659
   Numbers k such that k < p^2 * q, where p = smallest prime not dividing k
   and q = second smallest prime not dividing k.  The function returns
   the n‑th element (a(1)=1).  The implementation precomputes primes
   up to a fixed limit and builds the sequence on demand, caching results
   across multiple calls.  It works for any seq_t and idx_t definitions
   supplied by the build. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static state – cached terms and prime list */
static seq_t *terms = NULL;          /* array of sequence values          */
static size_t terms_len = 0;         /* number of terms computed so far  */
static size_t terms_cap = 0;         /* capacity of terms array          */
static idx_t next_k = 1;             /* next k to be tested              */

static uint64_t *primes = NULL;      /* array of primes                  */
static size_t primes_cnt = 0;        /* number of primes                 */
static const idx_t PRIME_LIMIT = 1000;  /* safety upper bound for primes   */

static void init_primes(void)
{
    /* primes are built once at the beginning or when more are needed */
    if (primes != NULL)
        return;
    build_primes_upto(PRIME_LIMIT, &primes, &primes_cnt);
}

/* Return the n-th term of A382659                                   */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */

    init_primes();                      /* ensure primes exist */

    /* grow the sequence until we have at least n terms       */
    while (terms_len < (size_t)n)
    {
        idx_t k = next_k;                /* current candidate          */
        uint64_t p = 0, q = 0;

        /* find the smallest prime not dividing k (p)          */
        for (size_t pi = 0; pi < primes_cnt; ++pi)
        {
            uint64_t prime = primes[pi];
            if (k % prime != 0)          /* first missing prime      */
            {
                p = prime;
                /* find the second smallest missing prime (q)   */
                for (size_t pj = pi + 1; pj < primes_cnt; ++pj)
                {
                    uint64_t prime2 = primes[pj];
                    if (k % prime2 != 0)  /* second missing prime     */
                    {
                        q = prime2;
                        break;
                    }
                }
                break;
            }
        }

        /* safety: if p or q not found, primes list is insufficient */
        /* (won't happen for the expected ranges)                    */
        if (!p || !q)
        {
            /* enlarge prime list (unlikely to be needed)           */
            idx_t new_limit = PRIME_LIMIT * (++primes_cnt);
            free(primes);
            build_primes_upto(new_limit, &primes, &primes_cnt);
            continue;
        }

        /* compute m = p^2 * q and test k < m                     */
        seq_t kval = (seq_t)k;
        seq_t m = (seq_t)p * (seq_t)p * (seq_t)q;
        if (kval < m)
        {
            if (terms_len == terms_cap)
                grow_seq(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = kval;
        }

        ++next_k;   /* advance to the next k for future calls */
    }

    return terms[n - 1];  /* zero‑based indexing */
}
