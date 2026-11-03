/*
 * A371267  Lexicographically latest sequence of distinct nonnegative integers
 *          such that the run lengths transforms of the balanced ternary
 *          expansions of n and of a(n) are the same.
 * https://oeis.org/A371267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 134, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Validated: standard, pass, 84 terms through n = 8444 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic computation of OEIS sequence A371267.
   The sequence is defined by:
   - Compute the run‑length signature of the balanced ternary expansion of n.
   - All numbers sharing a signature form a class.
   - Within each class, list the numbers in ascending order and map each number
     to the one that is symmetric with respect to the center of the list
     (the “reverse” pairing).
   - The two special numbers 0 and 1 are fixed points (they form a class of size 2
     but are left unmapped).
   See OEIS A371267 for details. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* compare two idx_t values for qsort */
static int cmp_idx(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Compute the run‑length signature of n.
   The signature is stored in 'runs', with 'run_cnt' the number of runs. */
static void compute_runs(idx_t n, int *runs, int *run_cnt)
{
    if (n == (idx_t)0) {
        runs[0] = 1;
        *run_cnt = 1;
        return;
    }

    int digits[64];
    int dcnt = 0;
    idx_t x = n;
    while (x > (idx_t)0) {
        int rem = (int)(x % (idx_t)3);
        x /= (idx_t)3;
        int d;
        if (rem == 0)        d = 0;
        else if (rem == 1)   d = 1;
        else {               d = -1;   /* rem == 2 */
                            x += (idx_t)1; }
        digits[dcnt++] = d;                     /* least significant first */
    }

    int prev = digits[dcnt - 1];
    int runlen = 1;
    *run_cnt = 0;
    for (int i = dcnt - 2; i >= 0; --i) {
        int cur = digits[i];
        if (cur == prev)
            ++runlen;
        else {
            runs[(*run_cnt)++] = runlen;
            prev = cur;
            runlen = 1;
        }
    }
    runs[(*run_cnt)++] = runlen; /* last run */
}

/* Determine whether x belongs to A003462 – sum of distinct powers of 3. */
static bool in_set(idx_t x)
{
    while (x) {
        if (x % 3 == 2) return false;
        x /= 3;
    }
    return true;
}

/* Recursive generation of all non‑negative integers sharing a given signature.
   The generated values are stored in 'out', and *cnt receives the number
   generated.  The array 'pow3_rev' contains powers of 3 in reverse order. */
static void dfs_gen(const int *runs, int run_cnt, int run_idx,
                    int prev_digit, int pos, int64_t val,
                    idx_t *out, size_t *cnt, const idx_t *pow3_rev)
{
    if (run_idx == run_cnt) {
        if (val >= 0) out[(*cnt)++] = (idx_t)val;
        return;
    }

    int len = runs[run_idx];
    for (int d = -1; d <= 1; ++d) {
        if (run_idx == 0 && d == 0) continue;           /* leading zero not allowed */
        if (prev_digit != 2 && d == prev_digit) continue;/* adjacent runs must differ */

        int64_t sum = val;
        for (int t = 0; t < len; ++t)
            sum += (int64_t)d * (int64_t)pow3_rev[pos + t];

        dfs_gen(runs, run_cnt, run_idx + 1, d, pos + len,
                sum, out, cnt, pow3_rev);
    }
}

static void generate_group(const int *runs, int run_cnt,
                           idx_t *out, size_t *cnt)
{
    int D = 0;
    for (int i = 0; i < run_cnt; ++i) D += runs[i];

    idx_t pow3_rev[64];
    idx_t p = 1;
    for (int i = D - 1; i >= 0; --i) { pow3_rev[i] = p; p *= 3; }

    *cnt = 0;
    dfs_gen(runs, run_cnt, 0, 2, 0, 0, out, cnt, pow3_rev);
}

/* Comparator for qsort on idx_t. */

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    /* 0 and 1 are fixed points. */
    if (n == (idx_t)0 || n == (idx_t)1) return (seq_t)n;

    int runs[64];
    int run_cnt;
    compute_runs(n, runs, &run_cnt);

    /* Generate all numbers sharing this signature. */
    idx_t values[2048];          /* safe upper bound (2^10 = 1024). */
    size_t valcnt;
    generate_group(runs, run_cnt, values, &valcnt);

    /* Sort the list to determine the symmetric partner. */
    qsort(values, valcnt, sizeof(idx_t), cmp_idx);

    /* Locate n inside the sorted list. */
    size_t pos = 0;
    while (pos < valcnt && values[pos] != n) ++pos;
    if (pos == valcnt) return (seq_t)0;   /* should never happen */

    /* Determine partner according to the symmetric rule. */
    seq_t partner;
    if (valcnt == 1) {
        partner = (seq_t)n;
    } else if (valcnt == 2) {
        /* Only 0 and 1 avoid the swap; already handled above. */
        partner = (seq_t)values[1 - pos];
    } else {
        partner = (seq_t)values[valcnt - 1 - pos];
    }
    return partner;
}
