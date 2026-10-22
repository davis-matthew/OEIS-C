/*
 * A375828  a(1) = 1; for n > 1, a(n) is the smallest unused positive number
 *          such that (a(n-1) AND a(n)) = a(n-1) if a(n-1) is prime, otherwise
 *          (a(n-1) AND a(n)) = 0, where AND is the binary AND operation.
 * https://oeis.org/A375828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest unused positive
 *            integer such that (a(n-1) AND a(n)) = a(n-1) if a(n-1) is prime,
 *            otherwise (a(n-1) AND a(n)) = 0
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

static seq_t *seq = NULL;          /* array of computed terms          */
static size_t seq_len = 0;         /* number of terms stored           */
static size_t seq_cap = 0;         /* capacity of seq array            */

static bool *used = NULL;          /* used[x] == true if number x seen*/
static size_t used_cap = 0;        /* capacity of used array           */

/* Ensure seq array can hold at least ‘need’ elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 2;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used array can index up to need‑1. */
static void ensure_used_cap(size_t need)
{
    if (used_cap >= need) return;
    size_t new_cap = used_cap ? used_cap * 2 : 2;
    if (new_cap < need) new_cap = need;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Initialise the sequence data structures. */
static void init_sequence(void)
{
    if (seq_len > 0) return;          /* already initialised */

    ensure_used_cap(2);                /* indices 0..1 */
    used[0] = false;
    used[1] = true;

    ensure_seq_cap(2);
    seq[0] = (seq_t)1;
    seq_len = 1;
    seq_cap = 2;
}

/* Compute a(n) for any n ≥ 1.  Memoised for arbitrary call order. */
seq_t a(idx_t n)
{
    assert(n >= 1);
    init_sequence();

    /* Extend sequence up to the requested index. */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        bool prev_is_prime = is_prime_big(prev);

        /* Find smallest unused candidate that satisfies the AND rule. */
        idx_t candidate = 1;
        seq_t cand_seq = 0;
        for (;;) {
            size_t c = (size_t)candidate;
            if (c >= used_cap) ensure_used_cap(c + 1);
            if (!used[c]) {
                cand_seq = (seq_t)candidate;
                if (prev_is_prime) {
                    if ((cand_seq & prev) == prev) break;
                } else {
                    if ((cand_seq & prev) == 0) break;
                }
            }
            ++candidate;
        }

        /* Append the found term. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = cand_seq;
        size_t c = (size_t)candidate;
        if (c >= used_cap) ensure_used_cap(c + 1);
        used[c] = true;
        ++seq_len;
    }

    return seq[n - 1];
}
