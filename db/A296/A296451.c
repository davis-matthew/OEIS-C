/*
 * A296451  Anagraprod Integers. Integers N that reproduce their multiset of
 *          digits when all the products of two successive digits of N are
 *          done (and considered together).
 * https://oeis.org/A296451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by exhaustive search of digit sequences satisfying
 *            the anagraprod property
 * Range:     u8 none, u16 n <= 46, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global workspace --------------------------------------------------- */
#define CAP 200000                /* maximum number of terms that can be stored */

static seq_t   results[CAP];      /* stored sequence values      */
static size_t  res_cnt = 0;       /* number of terms stored     */
static int     sorted_flag = 1;   /* 1 if results[0..res_cnt-1] sorted, 0 otherwise   */
static int     gen_len  = 2;      /* last length that has been generated (no numbers yet) */

/* helper arrays for DFS */
static int occ[10];               /* digit counts in the current number            */
static int prod[10];              /* counts of digits contributed by product edges */

/* comparison routine for qsort -------------------------------------- */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t*)a;
    seq_t vb = *(const seq_t*)b;
    if (va < vb) return -1;
    if (va > vb) return  1;
    return 0;
}

/* depth‑first search for numbers of exact length len ---------------- */
static void dfs(int pos, int len, seq_t val, int used_twod, int prev)
{
    if (pos == len)                 /* terminal – candidate finished   */
    {
        if (!used_twod) return;     /* need exactly one 2‑digit product  */

        for (int d = 0; d < 10; ++d)  /* check frequency equality   */
            if (occ[d] != prod[d]) return;

        if (res_cnt >= CAP) return; /* safety guard   */
        results[res_cnt++] = val;   /* store a valid term   */
        sorted_flag = 0;            /* array no longer sorted */
        return;
    }

    for (int nd = 0; nd <= 9; ++nd) /* try every possible next digit  */
    {
        int p   = prev * nd;       /* product of consecutive digits   */
        int two = (p >= 10);       /* is it a 2‑digit product?       */
        if (two && used_twod) continue;      /* at most one two‑digit edge   */

        int new_used = used_twod | two;
        occ[nd]++;                     /* add new digit to number   */

        if (p < 10)                    /* single‑digit product      */
        {
            prod[p]++;                     /* use the product digit     */
            dfs(pos+1, len, val* ((seq_t)10) + (seq_t)nd,
                new_used, nd);
            prod[p]--;                     /* backtrack                */
        }
        else                           /* two‑digit product        */
        {
            int t = p / 10;
            int u = p % 10;
            prod[t]++; prod[u]++;             /* add both digits          */
            dfs(pos+1, len, val* ((seq_t)10) + (seq_t)nd,
                new_used, nd);
            prod[t]--; prod[u]--;             /* backtrack                */
        }

        occ[nd]--;                     /* backtrack digit supply   */
    }
}

/* generate all valid numbers of a given length -------------------------------- */
static void generate_len(int len)
{
    /* reset frequency counters */
    for (int i = 0; i < 10; ++i) occ[i] = prod[i] = 0;

    /* start with a non‑zero first digit */
    for (int d0 = 1; d0 <= 9; ++d0)
    {
        occ[d0] = 1;
        dfs(1, len, (seq_t)d0, 0, d0);
        occ[d0] = 0;
    }
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 – for n<1 the result is undefined */

    /* generate subsequent lengths until we have at least n terms */
    while (res_cnt < n)
    {
        generate_len(gen_len + 1);
        gen_len++;                     /* next length to generate    */
    }

    /* ensure the array is sorted before we index it */
    if (!sorted_flag)
    {
        qsort(results, res_cnt, sizeof(seq_t), cmp_seq);
        sorted_flag = 1;
    }

    return results[n-1];               /* a(1) is the first element   */
}
