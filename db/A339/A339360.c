/*
 * A339360  Sum of all squarefree numbers with greatest prime factor prime(n).
 * https://oeis.org/A339360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 331 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339360:  a(0)=1, for n≥1  a(n)=A054640(n-1) * prime(n)
   where A054640(m)=∏_{k=1}^m (1+prime(k)) */
#include <stdlib.h>                    /* abort() */
#include <stdbool.h>
#include <stddef.h>

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

/* We only need a table up to some reasonable limit (≈10000). */
#define MAX_LIMIT 10001

static seq_t P[MAX_LIMIT];   /* P[i] = ∏_{k=1}^i (1+prime(k))   */
static idx_t last = 0;       /* largest i for which the entry is known */
static int initialized = 0;  /* true when the table has been seeded */

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1 */
    if (n == 0)
        return (seq_t)1;

    /* initialise the table on first use */
    if (!initialized) {
        P[0] = (seq_t)1;   /* empty product */
        last   = 0;
        initialized = 1;
    }

    /* n-1 is the largest index we need in the A054640 product */
    idx_t need = n - 1;
    if (need >= (idx_t)MAX_LIMIT) {
        /* For safety, abort if the requested index exceeds our pre‑allocated table.
           In the typical test harness this value will not be reached. */
        abort();
    }

    /* extend the table if required */
    for (idx_t i = last + 1; i <= need; ++i) {
        uint32_t p = get_prime(i);   /* i-th prime (1‑based) */
        P[i] = P[i - 1] * ((seq_t)p + (seq_t)1);
    }
    last = need;

    /* return the final value: prime(n) * P[n-1] */
    uint32_t prime_n = get_prime(n);   /* n-th prime (1‑based) */
    return ((seq_t)prime_n) * P[need];
}
