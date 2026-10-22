/*
 * A370222  Irregular triangle T(n,k) read by rows: row n gives the inversion
 *          table (see comments) of the permutation encoded by row n of
 *          A370221.
 * https://oeis.org/A370222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = flattened values of the inversion tables of
 *            stack‑sortable permutations of length m, sorted
 *            lexicographically
 * Range:     u8 n <= 8419, u16 n <= 8419, u32 n <= 8419, u64 n <= 8419,
 *            u128 n <= 8419, big any n
 * Helpers:   catalan_uint64, decode_permutation, generate_dyck_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan_uint64.h"
#include "../utils/decode_permutation.h"
#include "../utils/generate_dyck_rec.h"

/* Global cache for the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap = 0;
static idx_t  current_m   = 0;

/* Ensure the cache has room for at least 'needed' entries */
static void ensure_seq_cache_capacity(size_t needed)
{
    if (needed <= seq_cache_cap) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = new_buf;
    seq_cache_cap = new_cap;
}

/* Comparator for qsort – sorts permutations lexicographically. */
static int compare_m_global = 0;
static int compare_perm(const void *a, const void *b)
{
    const int *pa = *(const int **)a;
    const int *pb = *(const int **)b;
    int m = compare_m_global;
    for (int i = 0; i < m; ++i)
    {
        int x = pa[i], y = pb[i];
        if (x < y) return -1;
        if (x > y) return 1;
    }
    return 0;
}

/* Generate all rows for a specific length m, append inversion‑table
   entries to the global cache. */
static void generate_rows_for_m(int m)
{
    size_t cat = catalan_uint64(m);
    int **perm_ptrs = (int **)malloc(cat * sizeof(int *));
    size_t perm_count = 0;
    char *buf = (char *)malloc(2 * m + 1);

    generate_dyck_rec(m, 0, 0, 0, buf, perm_ptrs, &perm_count);
    free(buf);

    compare_m_global = m;
    qsort(perm_ptrs, perm_count, sizeof(int *), compare_perm);

    ensure_seq_cache_capacity(seq_cache_size + perm_count * m);

    int *pos = (int *)malloc((m + 1) * sizeof(int));
    for (size_t idx = 0; idx < perm_count; ++idx)
    {
        int *perm = perm_ptrs[idx];
        for (int i = 0; i < m; ++i) pos[perm[i]] = i;

        for (int k = 1; k <= m; ++k)
        {
            int cnt = 0;
            int pk = pos[k];
            for (int j = 1; j < k; ++j)
                if (pos[j] > pk) ++cnt;
            seq_cache[seq_cache_size++] = (seq_t)cnt;
        }
        free(perm);
    }
    free(pos);
    free(perm_ptrs);
}

/* Ensure that the cache contains at least 'n' entries. */
static void ensure_cache_size(idx_t n)
{
    while (seq_cache_size < n)
    {
        ++current_m;
        generate_rows_for_m((int)current_m);
    }
}

/* Public function producing the sequence. */
seq_t a(idx_t n)
{
    if (seq_cache == NULL)   /* lazy initialization */
    {
        seq_cache_cap = 0;
        seq_cache_size = 0;
        seq_cache = NULL;
        current_m = 0;
    }
    if (seq_cache_size < n) ensure_cache_size(n);
    return seq_cache[n - 1];
}
