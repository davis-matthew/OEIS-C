/*
 * A299996  Lexicographic first sequence of positive integers such that
 *          a(n)*a(n+1) has a digit 6, and no term occurs twice.
 * https://oeis.org/A299996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterative construction with memoization; 
   a(n) is the smallest unused positive integer such that 
   a(n)*a(n-1) contains digit 6.  The sequence is a permutation 
   of the positive integers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* 1‑based array of computed terms      */
static size_t seq_capacity = 0;    /* allocated size of seq                */
static idx_t seq_computed = 0;     /* last computed index                  */
static seq_t prev_val = 0;         /* a(n‑1)                                */

static uint64_t *used_bitmap = NULL; /* bitset of used integers          */
static size_t used_words = 0;        /* number of 64‑bit words in bitmap     */

static void ensure_used(size_t v)
{
    size_t need = v >> 6;
    if (need >= used_words) {
        size_t new_words = (need + 1) * 2;
        uint64_t *tmp = (uint64_t *)realloc(used_bitmap, new_words * sizeof(uint64_t));
        if (!tmp) return;          /* allocation failure ignored          */
        for (size_t i = used_words; i < new_words; ++i) tmp[i] = 0;
        used_bitmap = tmp;
        used_words = new_words;
    }
}

static inline bool is_used(size_t v)
{
    ensure_used(v);
    return (used_bitmap[v >> 6] >> (v & 63)) & 1ULL;
}

static inline void set_used(size_t v)
{
    ensure_used(v);
    used_bitmap[v >> 6] |= 1ULL << (v & 63);
}

static bool has_digit6(seq_t x)
{
    if (x == 0) return false;
    while (x) {
        if (x % 10 == 6) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Off‑spec: return 0 for n < 1        */
    /* Allocate initial space for the sequence array */
    if (!seq) {
        seq_capacity = 4; /* start with room for indices 0..3 */
        seq = (seq_t *)malloc(seq_capacity * sizeof(seq_t));
        seq[0] = 0;                 /* unused */
    }
    /* Compute terms until we reach the desired n */
    while (seq_computed < n) {
        ++seq_computed;
        if (seq_computed == 1) {
            seq[1] = 1;
            set_used(1);
            prev_val = 1;
            continue;
        }
        /* Ensure seq array has enough space */
        if (seq_computed + 1 >= seq_capacity) {
            size_t new_cap = seq_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure → undefined */
            seq = tmp;
            seq_capacity = new_cap;
        }
        seq_t cand = 1;
        for (;;) {
            if (!is_used((size_t)cand)) {
                seq_t prod = prev_val * cand;
                if (has_digit6(prod)) break;
            }
            ++cand;
        }
        seq[seq_computed] = cand;
        set_used((size_t)cand);
        prev_val = cand;
    }
    return seq[n];
}
