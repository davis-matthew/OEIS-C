/*
 * A320588  Derangements of {1,2,...,n} (n >= 2) in lexicographic order.
 * https://oeis.org/A320588
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) is the n‑th integer obtained by concatenating the
 *            lexicographically ordered derangements of {1,…,k} for the
 *            smallest k such that the cumulative count of derangements up to
 *            k-1 is < n-2. The function generates the required block of
 *            derangements once and returns the appropriate entry
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_alloc_new_array.h"

#define MAX_K 64

/* ---------- derangement counts ---------- */
static size_t der_dcount[MAX_K + 1] = {0};
static bool der_cnt_initialized = false;

static size_t der_count(int k)
{
    if (!der_cnt_initialized) {
        der_dcount[0] = 1;
        der_dcount[1] = 0;
        der_cnt_initialized = true;
    }
    for (int i = 2; i <= k; ++i) {
        if (der_dcount[i] == 0) {
            der_dcount[i] = (size_t)(i - 1) * (der_dcount[i - 1] + der_dcount[i - 2]);
        }
    }
    return der_dcount[k];
}

/* ---------- derangements storage ---------- */
static seq_t *der_list[MAX_K + 1] = {0};
static size_t der_len[MAX_K + 1] = {0};
static size_t der_cap[MAX_K + 1] = {0};

/* ---------- DFS generation of derangements ---------- */
static int perm[MAX_K];
static bool used[MAX_K + 1];

static void dfs_derange(int pos, int k)
{
    if (pos == k) {
        char num[3 * MAX_K + 1];
        char *p = num;
        for (int i = 0; i < k; ++i) {
            int e = perm[i];
            if (e < 10) {
                *p++ = '0' + e;
            } else {
                *p++ = '0' + e / 10;
                *p++ = '0' + e % 10;
            }
        }
        *p = '\0';
        der_list[k][der_len[k]++] = seq_from_str(num);
        return;
    }
    for (int d = 1; d <= k; ++d) {
        if (used[d]) continue;
        if (d == pos + 1) continue;     /* fixed point forbidden */
        used[d] = true;
        perm[pos] = d;
        dfs_derange(pos + 1, k);
        used[d] = false;
    }
}

static void generate_derangements(int k)
{
    if (der_cap[k]) return;            /* already generated */

    size_t count = der_count(k);
    der_cap[k] = count;
    der_list[k] = seq_alloc_new_array(count);
    der_len[k] = 0;

    for (int i = 0; i <= k; ++i) used[i] = false;

    dfs_derange(0, k);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* undefined for n < 2 */

    idx_t idx = n - 2;                    /* zero‑based position among all derangements */
    int k = 2;

    /* locate the block that contains the desired term */
    while (k <= MAX_K) {
        size_t dcnt = der_count(k);
        if (idx < (idx_t)dcnt) break;
        idx -= (idx_t)dcnt;
        ++k;
    }

    if (k > MAX_K) return (seq_t)0;       /* n too large for this build */

    if (!der_cap[k]) generate_derangements(k);
    return der_list[k][(size_t)idx];
}
