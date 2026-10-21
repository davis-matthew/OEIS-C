/*
 * A288164  Lexicographically earliest sequence of distinct positive terms
 *          such that, for any n > 0, a(n)*a(n+2) has at least 5 distinct
 *          prime factors.
 * https://oeis.org/A288164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9616, u32 n <= 9616, u64 n <= 9616,
 *            u128 n <= 9616, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A288164 – Lexicographically earliest permutation of ℕ such that
   for all n>0, the product a(n)*a(n+2) has at least 5 distinct
   prime factors.  The implementation builds the sequence on demand
   by a constructive search.  No hard‑coded table is used.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

/* ---------- dynamic arrays for the sequence and the used flag ---------- */
static seq_t *seq      = NULL;   /* zero‑based array of values              */
static size_t seq_len  = 0;      /* current number of elements in seq      */
static size_t seq_cap  = 0;      /* allocated capacity of seq              */

static bool *used      = NULL;   /* used[v] == true iff integer v used     */
static size_t used_cap = 0;      /* allocated capacity of used             */

/* grow the seq array to at least 'new_cap' elements */
static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap << 1 : 16;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq, cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = cap;
}

/* grow the used array to at least 'new_cap' indices (0..new_cap) */
static void grow_used(size_t new_cap)
{
    if (new_cap <= used_cap) return;
    size_t cap = used_cap ? used_cap << 1 : 16;
    while (cap < new_cap) cap <<= 1;
    bool *tmp = (bool*)realloc(used, (cap + 1) * sizeof(bool));
    if (!tmp) abort();
    /* initialise newly allocated part with false */
    for (size_t i = used_cap + 1; i <= cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = cap;
}

/* --------------------------------------------------------------------- */
/* return true iff the product of a and b has at least 5 distinct primes  */
static bool product_has_at_least_5_primes(seq_t a, seq_t b)
{
    uint64_t va = (uint64_t)a;
    uint64_t vb = (uint64_t)b;

    uint64_t primes_a[64];
    uint64_t exps_a[64];
    size_t ca = 0;
    factor_u64(va, primes_a, exps_a, &ca);

    uint64_t primes_b[64];
    uint64_t exps_b[64];
    size_t cb = 0;
    factor_u64(vb, primes_b, exps_b, &cb);

    /* count union of primes (duplicates ignored) */
    size_t total = ca;
    for (size_t j = 0; j < cb; ++j) {
        bool found = false;
        for (size_t i = 0; i < ca; ++i) {
            if (primes_b[j] == primes_a[i]) { found = true; break; }
        }
        if (!found) ++total;
    }
    return total >= 5;
}

/* --------------------------------------------------------------------- */
/* Build the sequence up to index n (1‑based).  The array seq is
   maintained on demand.  */
static void build_until(idx_t n)
{
    if (seq_len == 0) {
        /* initialise with a(1)=1, a(2)=2 */
        grow_seq(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        grow_used(2);
        used[1] = true;
        used[2] = true;
        seq_len = 2;
    }
    if (seq_len >= n) return;   /* already built */

    /* extend to n */
    for (idx_t pos = seq_len + 1; pos <= n; ++pos) {
        idx_t candidate = 1;
        for (;;) {
            grow_used((size_t)candidate);
            if (!used[(size_t)candidate]) {
                if (pos >= 3) {
                    seq_t prev = seq[(size_t)pos - 3];   /* a(pos-2) */
                    if (product_has_at_least_5_primes(prev, (seq_t)candidate))
                        break;   /* candidate accepted */
                } else {
                    break;   /* for pos 1 or 2 we already have a(1)=1 and a(2)=2 */
                }
            }
            ++candidate;
        }
        /* store candidate */
        grow_seq(seq_len + 1);
        seq[seq_len] = (seq_t)candidate;
        grow_used((size_t)candidate);
        used[(size_t)candidate] = true;
        ++seq_len;
    }
}

/* --------------------------------------------------------------------- */
/* Public accessor – returns a(n) for n>=1.  The function is safe for
   any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    build_until(n);
    return seq[(size_t)n - 1];
}
