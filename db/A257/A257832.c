/*
 * A257832  Squarefree numbers that are k*A005117(k) for some k.
 * https://oeis.org/A257832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = k * A005117(k) for the n‑th k such that
 *              k is squarefree and gcd(k, A005117(k)) = 1.
 *
 * Implementation details:
 *   - Generates squarefree numbers on demand (A005117(k)).
 *   - Tests each candidate k for squarefreeness and
 *     coprimality with its corresponding A005117(k).
 *   - Stores found sequence values in a static array so that the function
 *     can be called in any order.
 *
 * The routine works for all supported seq_t/idx_t types.  Allocations
 * use C++‑style casts so the code compiles as C++17 as well as C11.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* ---------- Global (static) state for memoisation ---------- */
static seq_t *terms        = NULL;   /* array of sequence terms          */
static size_t terms_cap     = 0;      /* allocated capacity for terms      */
static size_t terms_len     = 0;      /* number of stored terms            */

static idx_t *squarefree   = NULL;   /* list of squarefree numbers         */
static size_t sf_cap        = 0;      /* allocated capacity for sf         */
static size_t sf_len        = 0;      /* number of stored squarefree nums  */

static idx_t next_candidate = 1;     /* next integer to test for sfness   */
static idx_t current_k      = 1;     /* next k value to evaluate         */

/* Ensure terms array has at least new_cap elements */
static void ensure_terms_capacity(size_t new_cap) {
    if (new_cap <= terms_cap) return;
    size_t cap = terms_cap ? (terms_cap << 1) : 128;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failure is fatal     */
    terms = tmp;
    terms_cap = cap;
}

/* Ensure squarefree array has at least new_cap elements */
static void ensure_sqfree_capacity(size_t new_cap) {
    if (new_cap <= sf_cap) return;
    size_t cap = sf_cap ? (sf_cap << 1) : 64;
    while (cap < new_cap) cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(squarefree, cap * sizeof(idx_t));
    if (!tmp) abort();
    squarefree = tmp;
    sf_cap = cap;
}

/* Simple squarefree test using trial division (works for both seq_t and idx_t) */
static bool is_squarefree_uint64(uint64_t n) {
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                if (++cnt > 1) return false;
            }
        }
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined; harmless value */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Ensure we have the first current_k squarefree numbers generated */
        while (sf_len < (size_t)current_k) {
            /* Find next squarefree integer */
            while (!is_squarefree_uint64((uint64_t)next_candidate)) {
                ++next_candidate;
            }
            ensure_sqfree_capacity(sf_len + 1);
            squarefree[sf_len++] = next_candidate;
            ++next_candidate;
        }

        /* Retrieve k-th squarefree number A005117(k) */
        idx_t k   = current_k;
        idx_t sfk = squarefree[(size_t)k - 1];  /* 0‑indexed */

        /* Check conditions: k squarefree and coprime with sfk */
        if (is_squarefree_uint64((uint64_t)k)) {
            seq_t g = gcd((seq_t)k, (seq_t)sfk);
            if (g == 1) {
                seq_t prod = (seq_t)k * (seq_t)sfk;
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = prod;
            }
        }
        ++current_k;   /* move to next k */
    }

    return terms[(size_t)n - 1];
}
