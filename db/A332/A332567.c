/*
 * A332567  T(n,k) is the k-th partition of n in graded reverse lexicographic
 *          ordering (A080577) encoded as concatenation of parts which are
 *          represented in (zeroless) bijective base-9 numeration (A052382)
 *          and separated by zeros; triangle T(n,k), n >= 0, 1 <= k <=
 *          A000041(n), read by rows.
 * https://oeis.org/A332567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 28, u64 n <= 193,
 *            u128 n <= 3504, big any n
 * Helpers:   find_kth_partition_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332567: a(n) is the decimal encoding of the k‑th partition of
 * the integer m in reverse lexicographic order, where m is found by
 * cumulative partition counts.  Each part is written in bijective base‑9
 * (digits 1‑9) and adjacent parts are separated by the digit 0.  The
 * empty partition (m = 0) is encoded as 0.  The function enumerates
 * partitions in that order and constructs the k‑th one for the given
 * index n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/find_kth_partition_rec.h"

/* ----- Partition numbers ------------------------------------------------ */

static idx_t *part_counts = NULL;
static size_t part_counts_size = 0;

/* Extend part_counts so that indices up to n are available. */
static void ensure_partition_counts(size_t n)
{
    if (n + 1 <= part_counts_size)
        return;

    size_t new_size = n + 1;
    idx_t *new_array = (idx_t *)realloc(part_counts,
                                        new_size * sizeof(idx_t));
    if (!new_array)
        exit(EXIT_FAILURE);

    for (size_t i = part_counts_size; i < new_size; ++i) {
        if (i == 0) {
            new_array[i] = 1;
        } else {
            long long sum = 0;
            for (int k = 1; ; ++k) {
                int g1 = k * (3 * k - 1) / 2;
                if (g1 > (int)i)
                    break;
                int sign = (k & 1) ? 1 : -1;
                sum += sign * (long long)new_array[i - g1];
                int g2 = k * (3 * k + 1) / 2;
                if (g2 > (int)i)
                    continue;
                sum += sign * (long long)new_array[i - g2];
            }
            new_array[i] = sum > 0 ? (idx_t)sum : (idx_t)0;
        }
    }
    part_counts = new_array;
    part_counts_size = new_size;
}

static idx_t get_partition_count(int n)
{
    ensure_partition_counts((size_t)n);
    return part_counts[n];
}

/* ----- k-th partition enumeration --------------------------------------- */

/* ----- Main sequence function ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* empty partition */

    /* 1. Find the integer m (row) and the position k within that row. */
    idx_t cum = 0;
    int m   = 0;
    while (1) {
        idx_t p_m = get_partition_count(m);
        if (cum + p_m > n)
            break;
        cum += p_m;
        ++m;
    }
    idx_t k = n - cum + 1;      /* 1‑based index inside row m */

    /* 2. Enumerate the k‑th partition of m. */
    int cur_parts[64];
    int kth_parts[64];
    int kth_len = 0;
    idx_t cnt    = 0;

    find_kth_partition_rec(m, m, 0, cur_parts, k, &cnt, &kth_len, kth_parts);

    /* 3. Encode this partition into a decimal integer. */
    seq_t res = 0;
    for (int i = 0; i < kth_len; ++i) {
        int val = kth_parts[i];

        /* bijective base‑9 representation (digits 1‑9) */
        int dig[32], dcnt = 0;
        do {
            int d = (val - 1) % 9 + 1;
            dig[dcnt++] = d;
            val = (val - 1) / 9;
        } while (val > 0);

        /* append digits from most significant to least */
        for (int j = dcnt - 1; j >= 0; --j)
            res = res * 10 + dig[j];

        if (i != kth_len - 1)   /* separator 0 between parts */
            res = res * 10;
    }

    return res;
}
