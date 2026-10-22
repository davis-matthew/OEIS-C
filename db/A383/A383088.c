/*
 * A383088  Numbers whose multiset of prime indices does not have all equal
 *          run-sums.
 * https://oeis.org/A383088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383088:
   Numbers whose multiset of prime indices does not have all equal
   run‑sums.  The function a(n) returns the n‑th term (offset = 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- caches and sieve data ------------------------------------- */
static seq_t *seq      = NULL; /* stored terms                      */
static size_t seq_cap  = 0;    /* capacity of seq array             */
static size_t seq_len  = 0;    /* number of cached terms             */
static idx_t  last_checked = 1;/* last integer examined              */

/* Smallest prime factor table and prime index map */
static idx_t *spf          = NULL; /* spf[i] = smallest prime factor of i */
static idx_t *prime_index  = NULL; /* prime_index[p] = 1‑based index of prime p */
static idx_t  sieve_limit  = 0;    /* maximum value for which tables are built */

/* ---- helper: grow array if needed ------------------------------- */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* ---- build or extend the sieve up to 'limit' -------------------- */
static void ensure_sieve(idx_t limit)
{
    if (sieve_limit >= limit) return;

    free(spf);
    free(prime_index);

    spf          = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));
    prime_index  = (idx_t *)calloc((size_t)(limit + 1), sizeof(idx_t));

    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            ++idx;
            prime_index[i] = idx;
            if (i > limit / i) continue;        /* avoid overflow in j loop */
            for (idx_t j = i * 2; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    sieve_limit = limit;
}

/* ---- test whether 'num' satisfies the criterion ---------------- */
static bool qualifies(idx_t num)
{
    if (num < 2) return false;

    idx_t x = num;
    size_t distinct = 0;
    seq_t first_val = 0;
    bool all_equal = true;

    while (x > 1) {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) { x /= p; ++e; }

        seq_t val = (seq_t)((uint64_t)prime_index[p] * e);

        if (distinct == 0) {
            first_val = val;
        } else {
            if (val != first_val) all_equal = false;
        }
        ++distinct;

        if (!all_equal && distinct >= 2) return true;
    }

    if (distinct < 2) return false; /* power of a single prime */
    return !all_equal; /* return true if not all run‑sums equal */
}

/* ---- ensure that at least 'n' terms are cached ---------------- */
static void ensure_terms(idx_t n)
{
    while (seq_len < n) {
        ++last_checked;
        if (last_checked > sieve_limit) ensure_sieve(last_checked * 2 + 1);

        if (qualifies(last_checked)) {
            grow_seq_array(&seq, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
}

/* ---- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq[n - 1];
}
