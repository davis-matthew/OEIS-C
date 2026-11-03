/*
 * A325265  Numbers with sum of omega-sequence > 4.
 * https://oeis.org/A325265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth number whose sum of omega‑sequence exceeds 4 (search)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   bigomega, factor_u64, is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Compute red(n):  product of the primes at the positions of the exponents of n */
static uint64_t red_uint64(uint64_t n) {
    /* factor n and obtain exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    /* compute product of primes(exponent) */
    unsigned __int128 prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = get_prime((idx_t)exps[i]);  /* exponent is 1‑based index */
        prod *= p;
    }
    return (uint64_t)prod;
}

/* Return the (a)-th term of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 0, return 0 */

    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t last_checked = 1;   /* last integer examined */

    /* generate terms until we have n of them */
    while (terms_len < (size_t)n) {
        ++last_checked;                     /* candidate integer */
        uint64_t cur = last_checked;
        seq_t sum = 0;
        while (true) {
            /* add Omega(cur) */
            sum += bigomega((idx_t)cur);
            /* stop if cur is prime */
            if (is_prime((idx_t)cur))
                break;
            /* go to next in the omega‑sequence */
            cur = red_uint64(cur);
        }
        if (sum > (seq_t)4) {
            append_candidate(&terms, &terms_len, &terms_cap,
                             (seq_t)last_checked);
        }
    }
    return terms[(size_t)n - 1];
}
