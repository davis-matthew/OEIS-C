/*
 * A272552  Numbers n such that n*prime(n) is a pandigital number containing
 *          digits 0-9 exactly once.
 * https://oeis.org/A272552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: full, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   digit_counts, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Returns the n‑th term (n ≥ 1) of OEIS A272552.
   A term is a number k such that k * prime(k) is a decimal 0‑9 pandigital
   (contains each digit 0‑9 exactly once). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/num_digits.h"
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Memoised array of found terms. */
    static seq_t *terms = NULL;      /* array of terms */
    static size_t terms_cnt = 0;     /* number of stored terms */
    static size_t terms_cap = 0;     /* capacity of the array */

    /* The next integer to test. */
    static idx_t next_k = 1;

    /* Find terms until we have at least n of them. */
    while (terms_cnt < n) {
        seq_t prod = (seq_t)next_k * (seq_t)get_prime(next_k);

        /* Only 10‑digit numbers can be 0‑9 pandigital. */
        if (num_digits(prod) == 10) {
            unsigned char cnt[10] = {0};

            digit_counts(prod, cnt);

            bool ok = true;
            for (int d = 0; d < 10; ++d) {
                if (cnt[d] != 1) { ok = false; break; }
            }
            if (ok) {
                /* Ensure array has enough space and store the term. */
                grow_seq(&terms, &terms_cap, terms_cnt + 1);
                terms[terms_cnt++] = next_k;
            }
        }
        ++next_k;
    }

    return terms[n - 1];
}
