/*
 * A335123  Minimum part of the n-th integer partition in Abramowitz-Stegun
 *          order (sum/length/lex); a(0) = 0.
 * https://oeis.org/A335123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A335123: Minimum part of the n‑th integer partition
   in Abramowitz‑Stegun order (sum/length/lex); a(0) = 0.  The function
   generates all integer partitions of each sum in ascending order of
   length, then lexicographically ascending, and returns the smallest
   part of the partition indexed by n. */
#include <stdlib.h>
#include <stdbool.h>

/* Helper structure to hold a partition. */
typedef struct {
    idx_t *parts;  /* array of parts, non‑increasing */
    idx_t len;     /* number of parts */
} part_t;

/* Comparison function for qsort: sort by length ascending, then lexicographic ascending. */
static int part_cmp(const void *a, const void *b)
{
    const part_t *pa = (const part_t *)a;
    const part_t *pb = (const part_t *)b;
    if (pa->len != pb->len)
        return (pa->len < pb->len) ? -1 : 1;
    for (idx_t i = 0; i < pa->len; ++i) {
        if (pa->parts[i] != pb->parts[i])
            return (pa->parts[i] < pb->parts[i]) ? -1 : 1;
    }
    return 0;
}

/* Recursive generation of all partitions of 'sum' with parts <= 'max_part'.
   The current partially built partition is stored in 'cur' array, of length 'depth'.
   Generated partitions are appended to '*arr', with '*cnt' counting them.
   '*cap' is the allocated capacity of '*arr'. */
static void gen_parts(idx_t sum, idx_t max_part, idx_t *cur, idx_t depth,
                      part_t **arr, size_t *cnt, size_t *cap)
{
    if (sum == 0) {
        /* Store a copy of the current partition */
        part_t p;
        p.len = depth;
        p.parts = (idx_t *)malloc(depth * sizeof(idx_t));
        for (idx_t i = 0; i < depth; ++i)
            p.parts[i] = cur[i];

        if (*cnt >= *cap) {
            size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
            part_t *tmp = (part_t *)realloc(*arr, new_cap * sizeof(part_t));
            if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
            *arr = tmp;
            *cap = new_cap;
        }
        (*arr)[*cnt] = p;
        ++(*cnt);
        return;
    }

    for (idx_t part = (sum < max_part ? sum : max_part); part > 0; --part) {
        cur[depth] = part;
        gen_parts(sum - part, part, cur, depth + 1, arr, cnt, cap);
    }
}

/* Main function computing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* target is the zero‑based index among the natural partitions (excluding empty). */
    idx_t target = n - 1;
    idx_t remaining = target;

    /* Buffer for recursion */
    idx_t *cur = (idx_t *)malloc(sizeof(idx_t) * 64);  /* 64 is enough for sums < 64 */
    if (!cur) exit(EXIT_FAILURE);

    for (idx_t sum = 1; ; ++sum) {
        /* Generate all partitions of 'sum' */
        part_t *parts = NULL;
        size_t cnt = 0, cap = 0;
        gen_parts(sum, sum, cur, 0, &parts, &cnt, &cap);

        if (remaining >= cnt) {
            /* Skip whole block of partitions for this sum */
            remaining -= cnt;
            /* Free allocated partitions */
            for (size_t j = 0; j < cnt; ++j)
                free(parts[j].parts);
            free(parts);
            continue;
        }

        /* We are inside this sum; sort partitions */
        qsort(parts, cnt, sizeof(part_t), part_cmp);

        /* Find the desired partition by index remaining */
        for (size_t i = 0; i < cnt; ++i) {
            if (remaining == 0) {
                seq_t result = (seq_t)parts[i].parts[parts[i].len - 1];
                /* Clean up */
                for (size_t j = 0; j < cnt; ++j)
                    free(parts[j].parts);
                free(parts);
                free(cur);
                return result;
            }
            --remaining;
        }

        /* Should not reach here */
        for (size_t j = 0; j < cnt; ++j)
            free(parts[j].parts);
        free(parts);
    }
}
