/*
 * A374629  Irregular triangle listing the leaders of maximal weakly
 *          increasing runs in the n-th composition in standard order.
 * https://oeis.org/A374629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the leaders of maximal weakly increasing runs of the
 *            n-th composition in standard order (A066099), concatenated
 *            row‑wise. This implementation converts each composition index to
 *            its parts via the binary representation of the index and then
 *            extracts the leaders in O(bit‑length) time. Sequen...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t              next_k     = 1;   /* next composition index to process   */
static size_t             leaders_len   = 0;   /* number of precomputed leaders        */
static size_t             leaders_cap   = 0;   /* allocated capacity of leaders array   */
static seq_t             *leaders       = NULL;/* array holding precomputed leaders     */

/* Expand the leaders array to accommodate at least new_len elements. */
static void
ensure_leaders_capacity(size_t new_len)
{
    if (new_len <= leaders_cap)
        return;
    size_t new_cap = leaders_cap ? leaders_cap * 2 : 128;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(leaders, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    leaders   = tmp;
    leaders_cap = new_cap;
}

/* Process composition with index k (k >= 1) and append its leaders
   to the global array. */
static void
add_leaders_from_k(idx_t k)
{
    /* Find positions (1‑based) of 1 bits in the binary representation of k. */
    int  pos[64];
    int  pos_cnt = 0;
    idx_t tmp = k;
    int  bit   = 0;
    while (tmp != 0) {
        if (tmp & (idx_t)1)
            pos[pos_cnt++] = bit + 1;
        tmp >>= 1;
        bit++;
    }

    /* k = 0 yields an empty composition – no leaders. */
    if (pos_cnt == 0)
        return;

    /* Compute successive differences = part sizes. */
    int  diff[64];
    diff[0] = pos[0];                 /* first part = first position */
    for (int i = 1; i < pos_cnt; ++i)
        diff[i] = pos[i] - pos[i-1];

    int part_cnt = pos_cnt;           /* number of parts equals number of positions */

    /* Scan parts in reversed order to identify leaders. */
    int prev = 0;
    for (int i = 0; i < part_cnt; ++i) {
        int part = diff[part_cnt - 1 - i]; /* reversed order */
        if (i == 0 || part < prev) {
            ensure_leaders_capacity(leaders_len + 1);
            leaders[leaders_len++] = (seq_t)part;
        }
        prev = part;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Initialise static data on first call. */
    static int initialized = 0;
    if (!initialized) {
        next_k     = 1;
        leaders    = NULL;
        leaders_len = 0;
        leaders_cap = 0;
        initialized = 1;
    }

    /* Precompute leaders until we have at least n+1 terms. */
    while (leaders_len <= (size_t)n) {
        add_leaders_from_k(next_k);
        next_k++;
    }
    return leaders[n];
}
