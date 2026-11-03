/*
 * A376417  a(n) = n - A276076(A276075(n)), where A276075 and A276076 are
 *          factorial base log and exp-functions.
 * https://oeis.org/A376417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 287, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9889 (2026-11-01)
 * Helpers:   build_primes_upto, factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n - A276076(A276075(n))
 *
 * The implementation follows the definition:
 *   1. Factor n into primes p_i^e_i.
 *   2. Convert the multiset {p_i^e_i} into a factorial‑base number
 *      A276075(n) by adding e_i×i! to the digit at position i
 *      (i = 1‑based index of p_i in the prime list) and carrying
 *      modulo (i+1) as in mixed radix.
 *   3. Convert the resulting factorial digits back into a product of
 *      primes (A276076) and subtract it from n.
 *   4. Return the difference.
 *
 * The routine is cache‑friendly and works for any build where
 * seq_t is an unsigned integer type (fixed width or arbitrary precision).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Utility functions provided by the framework */
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* --- Prime index helper ------------------------------------------------ */

static uint64_t *global_primes = NULL;   /* array of primes up to global_limit */
static size_t   global_prime_cnt = 0;
static uint64_t global_limit      = 0;

/* Ensure that global_primes contains all primes <= limit. */
static void
ensure_primes_up_to(uint64_t limit)
{
    if (limit <= global_limit)
        return;

    free(global_primes);

    uint64_t *buf;
    size_t cnt;
    build_primes_upto(limit, &buf, &cnt);          /* allocate new list */
    global_primes      = buf;
    global_prime_cnt   = cnt;
    global_limit       = limit;
}

/* Return the 1‑based index of prime p, or 0 if p is not a prime. */
static int
prime_index_u64(uint64_t p)
{
    if (p < 2)
        return 0;

    ensure_primes_up_to(p);

    size_t lo = 0, hi = global_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (global_primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < global_prime_cnt && global_primes[lo] == p)
        return (int)(lo + 1);      /* indices are 1‑based */
    return 0;
}

/* --- Sequence definition ----------------------------------------------- */

seq_t
a(idx_t n)
{
    /* ---------- 1. Factor the input --------------------------------- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   fact_cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &fact_cnt);

    /* ---------- 2. Build factorial‑base digits --------------------- */
    int capacity   = 64;                  /* initial digit array size   */
    int *digits    = (int *)calloc(capacity, sizeof(int));
    if (!digits)
        return (seq_t)0;                  /* unlikely, but safe */
    int used_max   = 0;                    /* highest index seen so far */

    for (size_t k = 0; k < fact_cnt; ++k) {
        uint64_t  p    = primes[k];
        int       idx  = prime_index_u64(p);   /* 1‑based prime index      */
        if (idx == 0)
            continue;                   /* safety: non‑prime factor */

        /* Grow the array if needed. */
        if (idx >= capacity) {
            int old_cap = capacity;
            while (idx >= capacity)
                capacity <<= 1;
            int *tmp = (int *)realloc(digits, capacity * sizeof(int));
            if (!tmp) {
                free(digits);
                return (seq_t)0;            /* allocation failure */
            }
            digits = tmp;
            memset(digits + old_cap, 0,
                   (capacity - old_cap) * sizeof(int));
        }

        /* Add e × idx! to the factorial‑base representation. */
        digits[idx] += (int)exps[k];
        int j = idx;
        while (digits[j] >= j + 1) {          /* propagate carry */
            int carry = digits[j] / (j + 1);
            digits[j] %= (j + 1);
            ++j;
            if (j >= capacity) {
                int old_cap = capacity;
                capacity <<= 1;
                int *tmp = (int *)realloc(digits, capacity * sizeof(int));
                if (!tmp) {
                    free(digits);
                    return (seq_t)0;
                }
                digits = tmp;
                memset(digits + old_cap, 0,
                       (capacity - old_cap) * sizeof(int));
            }
            digits[j] += carry;
        }
        if (j > used_max)
            used_max = j;
    }

    /* ---------- 3. Convert digits to product of primes ------------ */
    seq_t prod = (seq_t)1;
    for (int i = 1; i <= used_max; ++i) {
        int d = digits[i];
        if (d == 0)
            continue;

        uint32_t p32 = get_prime((idx_t)i);      /* 1‑based index */
        seq_t    term = ipow_idx((seq_t)p32, (idx_t)d);
        prod = seq_mul(prod, term);
    }

    free(digits);

    /* ---------- 4. Final result ------------------------------------- */
    seq_t n_seq = (seq_t)n;
    seq_t result = n_seq - prod;
    return result;
}
