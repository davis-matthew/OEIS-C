/*
 * A383685  The lesser of two consecutive primes whose gap equals the
 *          difference between their digital roots.
 * https://oeis.org/A383685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383685: The lesser of two consecutive primes whose gap equals
   the difference between their digital roots. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *terms = NULL;        /* stored sequence terms */
static size_t terms_cap = 0;        /* capacity of the terms array */
static size_t terms_len = 0;        /* number of terms currently stored */
static idx_t prime_idx = 1;         /* index of the first prime in the next pair */

/* Compute the digital root of a number as a seq_t value. */
static seq_t digital_root(seq_t x)
{
    const seq_t TEN = 10;
    while (x >= TEN) {
        x = digit_sum(x);       /* repeated digit sum until a single digit */
    }
    return x;
}

/* Append a new term to the terms array, growing it if necessary. */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 8 : terms_cap << 1;
        terms = seq_realloc(terms, terms_cap, new_cap);
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Main sequence function.  The function may be called in any order;
   it lazily generates terms until the requested index is available. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, n<1 is undefined */

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t p = get_prime(prime_idx);     /* first prime of the pair */
        seq_t q = get_prime(prime_idx + 1); /* second prime of the pair */
        seq_t diff = q - p;                /* prime gap */

        seq_t dr_p = digital_root(p);
        seq_t dr_q = digital_root(q);

        /* Condition: gap equals the difference of digital roots. */
        if (dr_q >= dr_p && (dr_q - dr_p) == diff) {
            append_term(p);                /* store the lesser prime */
        }
        prime_idx++;                       /* move to next overlapping pair */
    }

    return terms[n - 1]; /* return the n‑th term (1‑based indexing) */
}
