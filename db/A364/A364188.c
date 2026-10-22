/*
 * A364188  The sum of the digits present in a(n) and a(n+1) divides exactly
 *          the product of the same digits. This is the lexicographically
 *          earliest sequence of distinct positive terms with this property.
 * https://oeis.org/A364188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   digit_counts, digit_product, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(1)=1; for n>1, a(n) is the lexicographically earliest unused
   positive integer such that the sum of the decimal digits of a(n-1) and a(n)
   divides the product of those digits.  Sequence terms are distinct. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/digit_counts.h"

/* Memoisation state */
static seq_t *seq = NULL;        /* stored sequence terms */
static size_t seq_cap = 0;       /* allocated capacity of seq */
static size_t seq_len = 0;       /* current number of terms */

static bool *used = NULL;        /* used numbers flag array */
static size_t used_cap = 0;      /* capacity of used array */

/* Ensure seq has capacity for at least n elements */
static void ensure_seq_capacity(size_t n)
{
    if (n <= seq_cap)
        return;
    size_t cap = seq_cap ? seq_cap * 2 : 64;
    while (cap < n)
        cap *= 2;
    seq_t *new_seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;
    seq_cap = cap;
}

/* Ensure used array has capacity for at least n indices (0..n-1) */
static void ensure_used_capacity(size_t n)
{
    if (n <= used_cap)
        return;
    size_t cap = used_cap ? used_cap * 2 : 64;
    while (cap < n)
        cap *= 2;
    bool *new_used = (bool *)realloc(used, cap * sizeof(bool));
    if (!new_used) abort();
    /* initialise new cells to false */
    memset(new_used + used_cap, 0, (cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = cap;
}

/* Check whether a non‑zero positive integer has digit 0 */
static bool has_zero_digit(seq_t x)
{
    uint8_t cnt[10];
    digit_counts(x, cnt);
    return cnt[0] != 0;
}

/* Generate terms until we have at least n terms (n >= 1) */
static void generate_up_to(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    if (seq_len == 0) {                /* initialise first term */
        ensure_seq_capacity(1);
        seq[0] = (seq_t)1;
        seq_len = 1;
        ensure_used_capacity(2);       /* we need index 1 */
        used[1] = true;
    }

    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        bool prev_has_zero = has_zero_digit(prev);

        /* candidate loop */
        unsigned long long candidate_ull = 1;
        for (;;) {
            if (candidate_ull == 0) {
                /* skip zero, only positive integers */
                ++candidate_ull;
                continue;
            }

            size_t idx = (size_t)candidate_ull;
            ensure_used_capacity(idx + 1);
            if (used[idx]) {
                ++candidate_ull;
                continue;
            }

            bool cand_has_zero = false;
            if (prev_has_zero) {
                cand_has_zero = true;          /* previous already has zero */
            } else {
                seq_t candidate = (seq_t)candidate_ull;
                cand_has_zero = has_zero_digit(candidate);
                if (cand_has_zero) {
                    /* property holds automatically */
                } else {
                    /* compute digit sums and products modulo S */
                    seq_t sum1 = digit_sum(prev);
                    seq_t sum2 = digit_sum((seq_t)candidate_ull);
                    uint64_t S = (uint64_t)sum1 + (uint64_t)sum2;
                    if (S == 0) {               /* should never happen */
                        cand_has_zero = false;
                    } else {
                        uint64_t p1 = (uint64_t)(digit_product(prev) % (seq_t)S);
                        uint64_t p2 = (uint64_t)(digit_product((seq_t)candidate_ull) % (seq_t)S);
                        uint64_t prod_mod = (p1 * p2) % S;
                        cand_has_zero = (prod_mod == 0);
                    }
                }
            }

            if (cand_has_zero) {
                /* accept candidate */
                ensure_seq_capacity(seq_len + 1);
                seq[seq_len] = (seq_t)candidate_ull;
                seq_len++;
                used[idx] = true;
                break;
            }
            ++candidate_ull;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        abort();                       /* invalid index */
    generate_up_to(n);
    return seq[n - 1];
}
