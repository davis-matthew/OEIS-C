/*
 * A299755  Triangle read by rows in which row n is the strict integer
 *          partition with FDH number n.
 * https://oeis.org/A299755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangle of strict integer
 *            partitions read by rows. Each integer n has a unique
 *            decomposition into distinct FDH primes f(s). The part list of n
 *            is obtained by factoring n over the already known f(s) values
 *            and reading the parts in descending order. The function `a`
 *            com...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Helper structures and global state */
typedef struct {
    size_t cnt;   /* number of parts */
    idx_t *parts; /* array of part indices, sorted descending */
} PartInfo;

static idx_t *f_values = NULL;   /* FDH prime values, indexed by part-1 */
static size_t f_count = 0, f_cap = 0;

static PartInfo *partitions = NULL; /* partitions[ n ]  */
static size_t part_cap = 0, part_count = 0; /* part_count == next free index */

static size_t *prefix_parts = NULL; /* cumulative part counts */
static size_t prefix_cap = 0, prefix_count = 0; /* prefix_count == next free index */

/* ---------- Dynamic array helpers ---------- */
static void grow_f_values(void) {
    if (f_cap == 0) {
        f_cap = 128;
        f_values = (idx_t *)malloc(f_cap * sizeof(idx_t));
    } else if (f_count == f_cap) {
        f_cap <<= 1;
        f_values = (idx_t *)realloc(f_values, f_cap * sizeof(idx_t));
    }
}

static void grow_partitions(void) {
    if (part_cap == 0) {
        part_cap = 128;
        partitions = (PartInfo *)malloc(part_cap * sizeof(PartInfo));
    } else if (part_count == part_cap) {
        part_cap <<= 1;
        partitions = (PartInfo *)realloc(partitions, part_cap * sizeof(PartInfo));
    }
}

static void grow_prefix(void) {
    if (prefix_cap == 0) {
        prefix_cap = 128;
        prefix_parts = (size_t *)malloc(prefix_cap * sizeof(size_t));
    } else if (prefix_count == prefix_cap) {
        prefix_cap <<= 1;
        prefix_parts = (size_t *)realloc(prefix_parts, prefix_cap * sizeof(size_t));
    }
}

/* ---------- Recursive subset finder ---------- */
#define MAX_PARTS 64
static bool find_subset(idx_t target, size_t start_idx,
                       idx_t out_parts[], size_t *out_cnt)
{
    if (target == 1) {
        *out_cnt = 0;
        return true;
    }
    for (size_t i = start_idx; i < f_count; ++i) {
        idx_t fval = f_values[i];
        if (fval > target) break;
        if (target % fval == 0) {
            size_t subcnt;
            if (find_subset(target / fval, i + 1, out_parts + 1, &subcnt)) {
                out_parts[0] = (idx_t)(i + 1); /* part index = i+1 */
                *out_cnt = subcnt + 1;
                return true;
            }
        }
    }
    return false;
}

/* ---------- Partition computation ---------- */
static void compute_partition(idx_t n) {
    if (n == 1) {
        /* empty partition */
        if (partitions[1].parts) free(partitions[1].parts);
        partitions[1].cnt = 0;
        partitions[1].parts = NULL;
        return;
    }
    if (partitions[n].cnt != 0 || n == 1) return;   /* already computed */

    idx_t tmp_parts[MAX_PARTS];
    size_t tmp_cnt = 0;
    bool ok = find_subset(n, 0, tmp_parts, &tmp_cnt);
    idx_t *arr;
    if (ok) {
        /* tmp_parts currently sorted ascending; reverse to descending */
        for (size_t i = 0; i < tmp_cnt / 2; ++i) {
            idx_t t = tmp_parts[i];
            tmp_parts[i] = tmp_parts[tmp_cnt - 1 - i];
            tmp_parts[tmp_cnt - 1 - i] = t;
        }
        arr = (idx_t *)malloc(tmp_cnt * sizeof(idx_t));
        memcpy(arr, tmp_parts, tmp_cnt * sizeof(idx_t));
        partitions[n].cnt = tmp_cnt;
        partitions[n].parts = arr;
    } else {
        /* new part: n becomes f_{new} */
        grow_f_values();
        idx_t new_part = (idx_t)(f_count + 1);
        f_values[f_count++] = n;
        arr = (idx_t *)malloc(sizeof(idx_t));
        arr[0] = new_part;
        partitions[n].cnt = 1;
        partitions[n].parts = arr;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t q) {
    static bool initialized = false;
    static idx_t current_n = 1;     /* last integer processed */

    if (!initialized) {
        /* initial state */
        grow_f_values();    /* prepare f_values array */
        grow_partitions();  /* prepare partitions array */
        grow_prefix();      /* prepare prefix_parts array */

        /* indices 0 and 1 are unused / for n=1 */
        partitions[0].cnt = 0;
        partitions[0].parts = NULL;
        compute_partition(1);          /* empty partition for n=1 */
        part_count = 2;                /* next free index = 2 */

        prefix_parts[0] = 0;           /* cumulative count up to n=0 */
        prefix_parts[1] = 0;           /* cumulative count up to n=1 */
        prefix_count = 2;              /* next free index = 2 */

        initialized = true;
    }

    /* Extend computations until we have enough parts to cover query q */
    while (prefix_parts[prefix_count - 1] < q) {
        ++current_n;                             /* next n */
        if (current_n >= part_count) grow_partitions();  /* ensure vector */
        compute_partition(current_n);
        size_t cnt = partitions[current_n].cnt;

        grow_prefix();
        prefix_parts[prefix_count++] =
            prefix_parts[prefix_count - 1] + cnt;
    }

    /* Locate the partition that contains the q-th part */
    idx_t lo = 1, hi = (idx_t)(prefix_count - 1);
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prefix_parts[mid] >= q)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t part_index = lo;                       /* n of the partition */
    idx_t prev_sum = prefix_parts[part_index - 1];
    idx_t offset = q - prev_sum - 1;             /* 0‑based offset within the partition */

    /* Retrieve the requested part */
    PartInfo *pi = &partitions[part_index];
    seq_t result = (seq_t)pi->parts[offset];
    return result;
}
