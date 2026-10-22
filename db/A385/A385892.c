/*
 * A385892  In the sequence of run lengths of binary indices of each positive
 *          integer (A245563), remove all duplicate rows after the first and
 *          take the last term of each remaining row.
 * https://oeis.org/A385892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the last run length of the binary indices of n with
 *            duplicate rows removed (OEIS A385892)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   compute_run_lengths, hash_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compute_run_lengths.h"
#include "../utils/hash_row.h"

typedef uint64_t idx_t;

/* Internal constants */
#define MAX_TERMS 200000
#define HASH_CAP (1 << 18)

/* Row structure: stores a unique run‑length pattern */
struct Row {
    idx_t *runs;      /* array of run lengths */
    size_t len;       /* number of run lengths */
};

/* Static storage for the sequence and helper data */
static seq_t results[MAX_TERMS];
static size_t results_count = 0;

static struct Row rows[MAX_TERMS];
static size_t rows_count = 0;

static size_t hash_to_row[HASH_CAP];
static idx_t processed_num = 0;
static bool initialized = false;

/* One‑time initialization of static data structures */
static void init_once(void) {
    if (initialized) return;
    for (size_t i = 0; i < HASH_CAP; ++i) {
        hash_to_row[i] = SIZE_MAX;   /* empty bucket sentinel */
    }
    initialized = true;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1 */
    init_once();

    while (results_count < (size_t)n) {
        ++processed_num;
        idx_t temp[64];
        size_t len;
        compute_run_lengths(processed_num, temp, &len);

        if (len == 0) continue;   /* should not happen for positive integers */

        uint64_t h = hash_row(temp, len);
        size_t idx = h & (HASH_CAP - 1);
        bool duplicate = false;
        size_t start_idx = idx;

        /* Probe hash table for duplicate */
        while (hash_to_row[idx] != SIZE_MAX) {
            size_t ridx = hash_to_row[idx];
            struct Row *r = &rows[ridx];
            if (r->len == len && memcmp(r->runs, temp, len * sizeof(idx_t)) == 0) {
                duplicate = true;
                break;
            }
            idx = (idx + 1) & (HASH_CAP - 1);
            if (idx == start_idx) break;  /* table full – should not occur */
        }

        if (!duplicate) {
            /* Store new unique row */
            size_t new_ridx = rows_count++;
            struct Row *newr = &rows[new_ridx];
            newr->len = len;
            newr->runs = (idx_t *)malloc(len * sizeof(idx_t));
            memcpy(newr->runs, temp, len * sizeof(idx_t));

            /* Insert into hash table */
            idx = h & (HASH_CAP - 1);
            while (hash_to_row[idx] != SIZE_MAX) {
                idx = (idx + 1) & (HASH_CAP - 1);
            }
            hash_to_row[idx] = new_ridx;

            /* Store last run length as next sequence value */
            results[results_count++] = (seq_t)newr->runs[len - 1];
        }
    }

    return results[n - 1];
}
