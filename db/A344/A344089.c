/*
 * A344089  Flattened tetrangle of reversed strict integer partitions, sorted
 *          first by length and then colexicographically.
 * https://oeis.org/A344089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all strict partitions of each integer t,
   sorting their parts first by length and then colexicographically.
   The sequence is formed by concatenating the parts of these
   partitions in that order.  */
#include <stdlib.h>
#include <string.h>

/* Partition representation */
struct partition {
    uint64_t *parts;   /* array of parts, ascending order */
    size_t len;        /* number of parts */
};

/* Dynamic list of partitions */
struct part_list {
    struct partition *data;   /* array of partitions */
    size_t size;              /* current number of partitions */
    size_t cap;               /* allocated capacity */
};

static void part_list_init(struct part_list *pl)
{
    pl->size = 0;
    pl->cap  = 4;
    pl->data = (struct partition *)malloc(pl->cap * sizeof(struct partition));
}

static void part_list_add(struct part_list *pl, const uint64_t *parts,
                          size_t len)
{
    if (pl->size == pl->cap) {
        pl->cap *= 2;
        pl->data = (struct partition *)realloc(pl->data,
                                               pl->cap * sizeof(struct partition));
    }
    struct partition *p = &pl->data[pl->size++];
    p->len = len;
    p->parts = (uint64_t *)malloc(len * sizeof(uint64_t));
    memcpy(p->parts, parts, len * sizeof(uint64_t));
}

/* Recursive generation of strict partitions of 'remaining' with
   the next part greater than 'last'. Parts are generated in
   ascending order. */
static void dfs_partitions(uint64_t remaining,
                           uint64_t last,
                           uint64_t *cur,
                           size_t cur_len,
                           struct part_list *pl)
{
    if (remaining == 0) {
        part_list_add(pl, cur, cur_len);
        return;
    }
    for (uint64_t next = last + 1; next <= remaining; ++next) {
        cur[cur_len] = next;
        dfs_partitions(remaining - next, next, cur, cur_len + 1, pl);
    }
}

/* Generate all strict partitions of integer t into strictly increasing parts */
static void generate_partitions_of_t(uint64_t t, struct part_list *pl)
{
    uint64_t *cur = (uint64_t *)malloc(t * sizeof(uint64_t));
    dfs_partitions(t, 0, cur, 0, pl);
    free(cur);
}

/* Comparator for sorting partitions:
   first by length ascending, then by colexicographic order. */
static int cmp_partitions(const void *a, const void *b)
{
    const struct partition *p1 = (const struct partition *)a;
    const struct partition *p2 = (const struct partition *)b;
    if (p1->len != p2->len)
        return (p1->len < p2->len) ? -1 : 1;
    /* colexicographic: compare from last element */
    for (size_t i = 0; i < p1->len; ++i) {
        uint64_t v1 = p1->parts[p1->len - 1 - i];
        uint64_t v2 = p2->parts[p2->len - 1 - i];
        if (v1 != v2)
            return (v1 < v2) ? -1 : 1;
    }
    return 0;
}

/* Row structure to cache partitions for a given integer t */
struct row_cache {
    struct part_list pl;       /* partitions of this t */
    uint64_t total_parts;      /* total number of elements in this row */
    uint64_t start_index;      /* index of the first element of this row */
};

seq_t a(idx_t n)
{
    /* Static cache of generated rows */
    static struct row_cache *rows = NULL;
    static size_t row_count = 0;
    static size_t row_cap   = 0;
    static uint64_t cumulative = 0;   /* first index of the next row */

    uint64_t target = (uint64_t)n;

    /* Generate rows until the target index is covered */
    while (cumulative <= target) {
        uint64_t t = (uint64_t)row_count + 1;   /* integer whose partitions form the new row */
        struct row_cache new_row;
        part_list_init(&new_row.pl);
        generate_partitions_of_t(t, &new_row.pl);
        qsort(new_row.pl.data, new_row.pl.size,
              sizeof(struct partition), cmp_partitions);

        /* compute total number of elements in this row */
        uint64_t row_total = 0;
        for (size_t i = 0; i < new_row.pl.size; ++i)
            row_total += new_row.pl.data[i].len;

        new_row.total_parts = row_total;
        new_row.start_index = cumulative;

        /* store in cache */
        if (row_count == row_cap) {
            row_cap = row_cap ? row_cap * 2 : 4;
            rows = (struct row_cache *)realloc(rows,
                                               row_cap * sizeof(struct row_cache));
        }
        rows[row_count++] = new_row;
        cumulative += row_total;
    }

    /* Locate the row containing the target index */
    size_t r = 0;
    while (r < row_count &&
           target >= rows[r].start_index + rows[r].total_parts)
        ++r;

    uint64_t offset = target - rows[r].start_index;

    /* Find the exact part within this row */
    struct part_list *pl = &rows[r].pl;
    for (size_t pi = 0; pi < pl->size; ++pi) {
        struct partition *p = &pl->data[pi];
        if (offset < p->len)
            return (seq_t)p->parts[offset];
        offset -= p->len;
    }

    /* Should never reach here */
    return (seq_t)0;
}
