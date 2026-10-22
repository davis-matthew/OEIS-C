/*
 * A340510  A permutation of the positive integers with a divisibility
 *          property (see Comments for precise definition).
 * https://oeis.org/A340510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = least unused positive integer such that sum_{i=1..n} a(i)
 *            ≡ 1 (mod n+1)
 * Range:     u8 n <= 21, u16 n <= 555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static cache of previously computed terms and a bitmap of used values. */
static seq_t *seq = NULL;                 /* seq[1..seq_len] */
static size_t seq_cap = 0;                /* allocated capacity for seq   */
static idx_t seq_len = 0;                 /* largest n computed           */

static uint8_t *used = NULL;              /* bitmap: used[x] == 1 iff x used */
static size_t used_cap = 0;               /* allocated size for used array */

static seq_t sum_seq = 0;                 /* running sum of seq[1..seq_len] */

/* Ensure seq has room for indices up to n (inclusive). */
static void ensure_seq_cap(idx_t n)
{
    if ((size_t)n + 1U <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1024;
    while (new_cap < (size_t)n + 1U) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used array can index up to value (inclusive). */
static void ensure_used_cap(size_t value)
{
    if (value <= used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1024;
    while (new_cap <= value) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
    if (!tmp) exit(1);
    /* initialise new slots to 0 */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* Return the n-th term of the sequence, n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range request */

    /* extend the precomputed range if necessary */
    if (n > seq_len) {
        ensure_seq_cap(n);
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            /* find the least unused integer satisfying the congruence */
            seq_t mod_val = (seq_t)(i + 1);          /* (i+1) as seq_t */
            seq_t rem     = sum_seq % mod_val;       /* sum % (i+1)    */
            seq_t target  = (mod_val - rem + 1) % mod_val; /* (1‑rem) mod m */
            seq_t candidate = target;
            if (candidate == 0) candidate = mod_val;      /* residue 0 → m */

            /* scan forward by multiples of mod_val until unused */
            while (true) {
                size_t idx = (size_t)candidate;
                ensure_used_cap(idx);
                if (!used[idx]) break;
                candidate += mod_val;
            }

            size_t idx = (size_t)candidate;
            ensure_used_cap(idx);
            used[idx] = 1;
            seq[i] = candidate;
            sum_seq += candidate;
        }
        seq_len = n;
    }
    return seq[n];
}
