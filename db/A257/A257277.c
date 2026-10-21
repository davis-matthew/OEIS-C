/*
 * A257277  a(n+1) has a digit that divides a(n) and is the least positive
 *          integer not appearing earlier with this property.
 * https://oeis.org/A257277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0) = 0; for n ≥ 0, a(n+1) is the least positive integer not
 *            yet * used that contains at least one decimal digit d with d |
 *            a(n). This * implementation builds the sequence lazily with O(n)
 *            time and space
 * Range:     u8 n <= 224, u16 n <= 1199, u32 n <= 1199, u64 n <= 1199,
 *            u128 n <= 1199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* Test whether x contains digit d (1‑9). */
static inline bool has_digit(seq_t x, int d)
{
    if (x == (seq_t)0) return false;           /* 0 has no 1‑9 digits */
    const seq_t ten = (seq_t)10;
    const seq_t dd  = (seq_t)d;
    while (x != (seq_t)0) {
        if (x % ten == dd) return true;
        x /= ten;
    }
    return false;
}

/* Dynamic array indicating whether a number has already appeared. */
static bool *used = NULL;
static size_t used_cap = 0;
static void ensure_used_capacity(size_t idx)
{
    if (idx >= used_cap) {
        size_t new_cap = (used_cap ? used_cap : 128);
        while (new_cap <= idx) new_cap <<= 1;
        bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }
}

/* Dynamic array storing the sequence values. */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len  = 0;   /* number of terms currently stored */
static void ensure_seq_capacity(size_t idx)
{
    if (idx >= seq_cap) {
        size_t new_cap = (seq_cap ? seq_cap : 64);
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq = tmp;
        seq_cap = new_cap;
    }
}

/* For each digit 1‑9, next_pos[d] holds the smallest unused number that
 * contains digit d. (next_pos[0] is unused.) */
static size_t next_pos[10] = {0};

/* One‑time initialization of static data structures. */
static void init_sequence(void)
{
    if (used != NULL) return;              /* already initialized */

    /* used array */
    used_cap = 128;
    used = (bool*)calloc(used_cap, sizeof(bool));
    used[0] = true;                       /* a(0) = 0 is already used */

    /* sequence array */
    seq_cap = 64;
    seq = (seq_t*)calloc(seq_cap, sizeof(seq_t));
    seq[0] = (seq_t)0;
    seq_len = 1;

    /* initialise next positions for digits 1‑9 */
    for (int d = 1; d <= 9; ++d) next_pos[d] = 1;
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    init_sequence();

    size_t target = (size_t)n;
    if (seq_len <= target) {
        /* Compute missing terms up to n. */
        for (size_t i = seq_len; i <= target; ++i) {
            /* previous value */
            seq_t prev = seq[i - 1];

            /* collect allowed digits d with d | prev. */
            int allow[9];
            int allow_cnt = 0;
            for (int d = 1; d <= 9; ++d) {
                if (prev % (seq_t)d == (seq_t)0) allow[allow_cnt++] = d;
            }

            /* find minimal candidate among allowed digits. */
            size_t best_candidate = (size_t)-1;
            for (int k = 0; k < allow_cnt; ++k) {
                int d = allow[k];
                size_t cand = next_pos[d];
                if (cand < best_candidate) best_candidate = cand;
            }

            /* store the new term. */
            ensure_seq_capacity(i);
            seq[i] = (seq_t)best_candidate;
            ++seq_len;

            /* mark the number as used. */
            ensure_used_capacity(best_candidate);
            used[best_candidate] = true;

            /* update next_pos for all digits. */
            for (int d = 1; d <= 9; ++d) {
                size_t p = next_pos[d];
                while (1) {
                    ensure_used_capacity(p);
                    if (!used[p] && has_digit((seq_t)p, d)) break;
                    ++p;
                }
                next_pos[d] = p;
            }
        }
    }
    return seq[target];
}
