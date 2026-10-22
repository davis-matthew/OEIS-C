/*
 * A334440  Irregular triangle T(n,k) read by rows: row n lists numbers of
 *          distinct parts of the n-th integer partition in Abramowitz-Stegun
 *          (sum/length/lex) order.
 * https://oeis.org/A334440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A001221(A334433(n))
 * Range:     u8 n <= 9295, u16 n <= 9295, u32 n <= 9295, u64 n <= 9295,
 *            u128 n <= 9295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* abort() */
#include <stddef.h>   /* size_t */

/* Maximum number of terms that can be stored.  The index requests of the
   problem are far below this limit, so a fixed static array suffices
   and avoids dynamic reallocation of possibly non‑trivial types. */
#define MAX_TERMS 200000
#define PARTS_MAX 512

static seq_t  A[MAX_TERMS];   /* flattened triangle */
static size_t A_len       = 0;
static idx_t  next_row    = 0;   /* next row to generate */

/* Append a value to the flattened array, aborting on overflow. */
static void add_value(seq_t val) {
    if (A_len == MAX_TERMS) abort();
    A[A_len++] = val;
}

/* Recursive generation of partitions of `rem` with at most `max_part`,
   where `depth` is the current position and `L` is the required
   length of the partition.  The array `parts` holds the parts
   during recursion.  For each complete partition we compute the
   number of distinct parts and append it to the global array `A`. */
static void gen_parts(idx_t rem, idx_t max_part, idx_t depth,
                      idx_t L, idx_t parts[]) {
    if (depth == L) {                     /* Should not happen for L>=1 */
        if (rem == 0) {
            seq_t distinct = 1;
            for (idx_t i = 1; i < L; ++i)
                if (parts[i] != parts[i-1]) distinct++;
            add_value(distinct);
        }
        return;
    }

    idx_t parts_left = L - depth - 1;
    idx_t bound      = rem < max_part ? rem : max_part;

    for (idx_t i = 1; i <= bound; ++i) {
        idx_t rem2 = rem - i;
        if (rem2 < parts_left) continue;          /* not enough sum left */
        if (rem2 > i * parts_left) continue;      /* parts too large */

        parts[depth] = i;

        if (depth + 1 == L) {                    /* last part chosen */
            if (rem2 == 0) {
                seq_t distinct = 1;
                for (idx_t j = 1; j < L; ++j)
                    if (parts[j] != parts[j-1]) distinct++;
                add_value(distinct);
            }
            /* else prune */
        } else {
            gen_parts(rem2, i, depth + 1, L, parts);
        }
    }
}

/* Generate all rows for a given `n` (the row index).  For each
   partition of `n` (in the required order) the number of distinct
   parts is computed and appended to the global array `A`. */
static void generate_row(idx_t n) {
    if (n == 0) {                     /* special case: empty partition */
        add_value((seq_t)0);
        return;
    }

    idx_t parts[PARTS_MAX];
    for (idx_t L = 1; L <= n; ++L) {          /* length ascending */
        gen_parts(n, n, 0, L, parts);
    }
}

/* Public sequence function, indexed as a(0), a(1), ...
   Generates rows on demand and returns the n‑th term. */
seq_t a(idx_t n) {
    if ((size_t)n < A_len)
        return A[n];

    while ((size_t)n >= A_len) {
        generate_row(next_row);
        ++next_row;
    }
    return A[n];
}
